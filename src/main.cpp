#include "dataunit.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <window.h>
#include <spdlog/spdlog.h>
#define DEBUG
#include <Base.h>

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

	File::Extracter file("db.rot");
	File::Parser parser(file);
    auto data = parser.Process();

	std::string bufferName;
	std::string bufferValue;

	int current_name = 0;
	
  	Window win(1280, 720, "Парковка");
    while (win.StartUpdate())
    {
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
					 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(win.GetWindowIO().DisplaySize);
		ImGui::SetWindowPos({0, 0});

	    if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				const char* preview_val = data.GetObjects()[current_name].first.c_str();
			    if (ImGui::BeginCombo("Names for commiting", preview_val))
				{
					auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
					for (size_t i = 0; i < names.size(); i++)
					{
						const bool is_selected = (current_name == i);
					    if (ImGui::Selectable(names[i].c_str(), is_selected))
						    current_name = i;
						
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::Text("Name: "); ImGui::SameLine(); ImGui::InputText("NewName", &bufferName);
				ImGui::Text("Value: "); ImGui::SameLine(); ImGui::InputText("NewValue", &bufferValue);
				if (ImGui::Button("Commit"))
				{
					data.AddIntoExisting(data.GetObjects()[current_name].first, {bufferName, bufferValue});
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
					CreateTableInput(object.second[i].value, i+10);
				}
				
				ImGui::EndTable();
			}
		}
		parser.BackProcess(data);
		
		ImGui::End();
		
        win.EndUpdate();
    }

	Window::StopIMGUI();

    return 0;
}
