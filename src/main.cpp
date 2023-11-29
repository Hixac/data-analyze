#include "dataunit.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <window.h>
#include <spdlog/spdlog.h>

#define DEBUG
#include <Base.h>

#include <imgui/fastcombo.h>
#include <parser.h>

void CreateTableInput(std::string& buffer, size_t id)
{
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::PushID(id);
	ImGui::InputText("##cell", &buffer);
	ImGui::PopID();
}

template<typename __first, typename __last>
std::vector<__first> GetFirstOfPairs(std::vector<std::pair<__first, __last>> data)
{
	std::vector<std::string> firsts;
	for (auto unit : data)
	{
		firsts.push_back(unit.first);
	}

	return firsts;
}

int main(void)
{
	INIT_LOG();

	/* PARKING WINDOW VARIABLES */
	
	File::Extracter file("db.rot");
	File::Parser parser(file);
	Database::Intersort data = parser.Process();

	Database::Object bufferObject;
    int bufferColumns = 1;
	
	std::string bufferName;
	std::string bufferValue;
	
	unsigned int current_name = 0;
	
  	Window win(1280, 720, "Парковка");
    while (win.StartUpdate())
    {
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
					 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({win.GetWindowIO().DisplaySize.x / 2, win.GetWindowIO().DisplaySize.y});
		ImGui::SetWindowPos({0, 0});

	    if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add object"))
			{
				ImGui::InputText("Object", &bufferObject.first);
				ImGui::SliderInt("Columns", &bufferColumns, 1, 100);

				for (size_t i = 0; bufferObject.second.size() < bufferColumns && i < bufferColumns; i++)
					bufferObject.second.push_back({});
				
				if (ImGui::BeginTable("Example", 2, ImGuiTableFlags_SizingStretchSame))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Value");
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

				if (ImGui::Button("Commit") && !bufferObject.first.empty())
				{
					data.Add(bufferObject.first, bufferObject.second);
				}
				
				ImGui::EndMenu();
			}
			else
			{
				bufferObject.first.clear(); bufferObject.second.clear();
			}

			if (ImGui::BeginMenu("Delete object"))
			{
			    const char* preview_val = data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
				MyGui::FastCombo("Object listing", names, current_name, preview_val);
				if (ImGui::Button("Commit"))
				{
					data.Delete(preview_val);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Add|Change"))
			{
				const char* preview_val = data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
				MyGui::FastCombo("Object listing", names, current_name, preview_val);
				
				ImGui::InputText("Name", &bufferName);
				ImGui::InputText("Value", &bufferValue);
				if (ImGui::Button("Commit"))
				{
					data.AddIntoExisting(data.GetObjects()[current_name].first, {bufferName, bufferValue});
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Delete"))
			{
				const char* preview_val = data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
				MyGui::FastCombo("Object listing", names, current_name, preview_val);
				
				ImGui::InputText("Name", &bufferName);
				if (ImGui::Button("Commit"))
				{
					data.Delete(data.GetObjects()[current_name].first, bufferName);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		for (auto& object : data.GetObjects())
		{
			ImGui::Text("%s", object.first.c_str()); ImGui::SameLine();
			if (ImGui::BeginTable("Data", 2))
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
		
		parser.BackProcess(data);
		
		ImGui::End();

		ImGui::Begin("Example", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({win.GetWindowIO().DisplaySize.x / 2, win.GetWindowIO().DisplaySize.y});
		ImGui::SetWindowPos({win.GetWindowIO().DisplaySize.x / 2, 0});

		std::string text = file.GetContent();
		ImGui::InputTextMultiline("##Example", &text, {win.GetWindowIO().DisplaySize.x / 2, win.GetWindowIO().DisplaySize.y});
		file.SetContent(text);

		data = parser.Process();
		
		ImGui::End();
		
        win.EndUpdate();
    }

	Window::StopIMGUI();

    return 0;
}
