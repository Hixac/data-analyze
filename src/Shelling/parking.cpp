#include "Shelling/mainshell.h"
#include "dataunit.h"
#include <Shelling/parking.h>

#include <implot.h>
#include <imgui/fastcombo.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <mathread.h>

namespace Shell {

	void Parking::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Parking::UpdateWrap()
	{
		ImGui::SetNextWindowBgAlpha(1);
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::SetWindowSize({(float)m_width, (float)m_height});
		ImGui::SetWindowPos({m_posx, m_posy});

		if (ImGui::BeginMenuBar())
		{
			static Database::Object bufferObject;
			static int bufferColumns = 1;

			static std::string bufferName;
			static std::string bufferValue;
			
			if (ImGui::BeginMenu("Добавить таблицу"))
			{
				Label("Таблица");
				ImGui::InputText("##Object", &bufferObject.first);
				Label("Колонны");
				ImGui::SliderInt("##Columns", &bufferColumns, 1, 100);

				if (bufferObject.second.size() > bufferColumns)
				    bufferObject.second.clear();
				
				for (size_t i = 0; bufferObject.second.size() < bufferColumns && i < bufferColumns; i++)
				    bufferObject.second.push_back({});
				
				if (ImGui::BeginTable("Пример", 2, ImGuiTalkBuffer::tableFlags))
				{
					ImGui::TableSetupColumn("Именование");
					ImGui::TableSetupColumn("Значение");
					ImGui::TableHeadersRow();

					ImGui::TableNextColumn();
					for (size_t i = 0; i < bufferColumns; i++)
					{
						CreateTableInput(bufferObject.second[i].name, i);
					}

					ImGui::TableNextColumn();
					for (size_t i = 0; i < bufferColumns; i++)
					{
						CreateTableInput(bufferObject.second[i].value, i+100);
					}
				
					ImGui::EndTable();
				}

				if (ImGui::Button("Добавить") && !bufferObject.first.empty() && !ImGuiTalkBuffer::data.FindCode(bufferObject.first))
				{
					ImGuiTalkBuffer::data.Add(bufferObject.first, bufferObject.second);
				}
				
				ImGui::EndMenu();
			}
			ImGui::Separator();
			static unsigned int current_name = 0;
			if (ImGui::BeginMenu("Удалить таблицу"))
			{
				const char* preview_val = ImGuiTalkBuffer::data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(ImGuiTalkBuffer::data.GetObjects());
				Label("Листинг");
				MyGui::FastCombo("##Object listing", names, current_name, preview_val);
				if (ImGui::Button("Удалить") && ImGuiTalkBuffer::data.GetObjects().size() - 1 != 0)
				{
					ImGuiTalkBuffer::data.Delete(preview_val);
				    current_name = 0;
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Добавить Изменить"))
			{
				current_name = 0;
				const char* preview_val = ImGuiTalkBuffer::data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(ImGuiTalkBuffer::data.GetObjects());
				Label("Листинг");
				MyGui::FastCombo("Таблицы", names, current_name, preview_val);

				Label("Name");
				ImGui::InputText("##Name", &bufferName);
				Label("Value");
				ImGui::InputText("##Value", &bufferValue);
				if (ImGui::Button("Фиксировать") && !bufferName.empty() && !bufferValue.empty())
				{
					ImGuiTalkBuffer::data.AddIntoExisting(ImGuiTalkBuffer::data.GetObjects()[current_name].first, {bufferName, bufferValue});
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Удалить"))
			{
				current_name = 0;
				const char* preview_val = ImGuiTalkBuffer::data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(ImGuiTalkBuffer::data.GetObjects());
				Label("Листинг");
				MyGui::FastCombo("##Object listing", names, current_name, preview_val);

				Label("Именование");
				ImGui::InputText("##Name", &bufferName);
				if (ImGui::Button("Удалить") && !bufferName.empty())
					ImGuiTalkBuffer::data.Delete(ImGuiTalkBuffer::data.GetObjects()[current_name].first, bufferName);
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Вид"))
			{
				for (int i = 1; i < ImGuiTalkBuffer::windows->size(); i++)
				{
					ImGui::Checkbox((*ImGuiTalkBuffer::windows)[i]->GetLabel().c_str(), &(*ImGuiTalkBuffer::windows)[i]->GetActive());
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			ImGui::EndMenuBar();
		}
		
		for (auto& object : ImGuiTalkBuffer::data.GetObjects())
		{
			ImGui::Text("%s", object.first.c_str());
			if (ImGui::BeginTable("Данные", 2, ImGuiTalkBuffer::tableFlags))
			{
				ImGui::TableSetupColumn("Именование");
				ImGui::TableSetupColumn("Значение");
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
