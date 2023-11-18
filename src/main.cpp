#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <window.h>
#include <spdlog/spdlog.h>
#define DEBUG
#include <Base.h>

#include <parser.h>

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

		if (ImGui::BeginTable("Data", 1))
		{
		    
			for (auto& it : data.GetObjects())
			{
				ImGui::TableNextColumn();
				ImGui::InputText("Label", &it.first);
			}
			
			ImGui::EndTable();
		}
		
		ImGui::End();
		
        win.EndUpdate();
    }

	Window::StopIMGUI();

    return 0;
}
