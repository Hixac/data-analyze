#include <Shelling/parking.h>

#include <imgui/fastcombo.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Shell {

	Parking::Parking(Window& window)
		: m_Window(&window)
	{}
	
	void Parking::OnUpdate()
	{
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
					 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({(m_Window->GetWindowIO().DisplaySize.x / 3) * 2, m_Window->GetWindowIO().DisplaySize.y - ImGuiTalkBuffer::winCheckSize});
		ImGui::SetWindowPos({0, 0});

		if (ImGui::BeginMenuBar())
		{	
			if (ImGui::BeginMenu("Add object"))
			{
				Label("Object");
				ImGui::InputText("##Object", &ImGuiTalkBuffer::bufferObject.first);
				Label("Columns");
				ImGui::SliderInt("##Columns", &ImGuiTalkBuffer::bufferColumns, 1, 100);

				if (ImGuiTalkBuffer::bufferObject.second.size() > ImGuiTalkBuffer::bufferColumns)
					ImGuiTalkBuffer::bufferObject.second.clear();
				
				for (size_t i = 0; ImGuiTalkBuffer::bufferObject.second.size() < ImGuiTalkBuffer::bufferColumns && i < ImGuiTalkBuffer::bufferColumns; i++)
					ImGuiTalkBuffer::bufferObject.second.push_back({});
				
				if (ImGui::BeginTable("Example", 2, ImGuiTalkBuffer::tableFlags))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Value");
					ImGui::TableHeadersRow();

					ImGui::TableNextColumn();
					for (size_t i = 0; i < ImGuiTalkBuffer::bufferColumns; i++)
					{
						CreateTableInput(ImGuiTalkBuffer::bufferObject.second[i].name, i);
					}

					ImGui::TableNextColumn();
					for (size_t i = 0; i < ImGuiTalkBuffer::bufferColumns; i++)
					{
						CreateTableInput(ImGuiTalkBuffer::bufferObject.second[i].value, i+100);
					}
				
					ImGui::EndTable();
				}

				if (ImGui::Button("Commit") && !ImGuiTalkBuffer::bufferObject.first.empty() && !ImGuiTalkBuffer::data.FindCode(ImGuiTalkBuffer::bufferObject.first))
				{
					ImGuiTalkBuffer::data.Add(ImGuiTalkBuffer::bufferObject.first, ImGuiTalkBuffer::bufferObject.second);
				}
				
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Delete object"))
			{
				const char* preview_val = ImGuiTalkBuffer::data.GetObjects()[ImGuiTalkBuffer::currentName].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(ImGuiTalkBuffer::data.GetObjects());
				Label("Listing");
				MyGui::FastCombo("##Object listing", names, ImGuiTalkBuffer::currentName, preview_val);
				if (ImGui::Button("Commit"))
				{
					ImGuiTalkBuffer::data.Delete(preview_val);
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Add Change"))
			{
				const char* preview_val = ImGuiTalkBuffer::data.GetObjects()[ImGuiTalkBuffer::currentName].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(ImGuiTalkBuffer::data.GetObjects());
				Label("Listing");
				MyGui::FastCombo("Object listing", names, ImGuiTalkBuffer::currentName, preview_val);

				Label("Name");
				ImGui::InputText("##Name", &ImGuiTalkBuffer::bufferName);
				Label("Value");
				ImGui::InputText("##Value", &ImGuiTalkBuffer::bufferValue);
				if (ImGui::Button("Commit") && !ImGuiTalkBuffer::bufferName.empty() && !ImGuiTalkBuffer::bufferValue.empty())
				{
					ImGuiTalkBuffer::data.AddIntoExisting(ImGuiTalkBuffer::data.GetObjects()[ImGuiTalkBuffer::currentName].first, {ImGuiTalkBuffer::bufferName, ImGuiTalkBuffer::bufferValue});
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Delete"))
			{
				const char* preview_val = ImGuiTalkBuffer::data.GetObjects()[ImGuiTalkBuffer::currentName].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(ImGuiTalkBuffer::data.GetObjects());
				Label("Listing");
				MyGui::FastCombo("##Object listing", names, ImGuiTalkBuffer::currentName, preview_val);

				Label("Name");
				ImGui::InputText("##Name", &ImGuiTalkBuffer::bufferName);
				if (ImGui::Button("Commit") && !ImGuiTalkBuffer::bufferName.empty())
				{
					ImGuiTalkBuffer::data.Delete(ImGuiTalkBuffer::data.GetObjects()[ImGuiTalkBuffer::currentName].first, ImGuiTalkBuffer::bufferName);
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			ImGui::EndMenuBar();
		}

		for (auto& object : ImGuiTalkBuffer::data.GetObjects())
		{
			ImGui::Text("%s", object.first.c_str());
			if (ImGui::BeginTable("Data", 2, ImGuiTalkBuffer::tableFlags))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Value");
				ImGui::TableHeadersRow();

				ImGui::TableNextColumn();
				for (size_t i = 0; i < object.second.size(); i++)
				{
					CreateTableInput(object.second[i].name, i);
				}

				ImGui::TableNextColumn();
				for (size_t i = 0; i < object.second.size(); i++)
				{
					CreateTableInput(object.second[i].value, i+1000);
				}
				
				ImGui::EndTable();
			}
		}
		
		ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
		
		ImGui::End();
	}

	void Parking::CreateTableInput(std::string& buffer, size_t id)
	{
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::PushID(id);
		ImGui::InputText("##cell", &buffer);
		ImGui::PopID();
	}

	template<typename __first, typename __last>
	std::vector<__first> Parking::GetFirstOfPairs(std::vector<std::pair<__first, __last>> data)
	{
		std::vector<std::string> firsts;
		for (auto unit : data)
		{
			firsts.push_back(unit.first);
		}

		return firsts;
	}
	
	void Parking::Label(const char* name, bool left)
	{
		ImGui::Text("%s", name);
		if (left) ImGui::SameLine();
	}
	
}
