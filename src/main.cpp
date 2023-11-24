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

int main(void)
{
	INIT_LOG();

	File::Extracter file("db.rot");
	File::Parser parser(file);
    auto data = parser.Process();
	
  	Window win(1280, 720, "Парковка");
    while (win.StartUpdate())
    {
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar
			| ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(win.GetWindowIO().DisplaySize);
		ImGui::SetWindowPos({0, 0});

	    if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				
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
		
		ImGui::End();
		
        win.EndUpdate();
    }

	Window::StopIMGUI();

    return 0;
}
