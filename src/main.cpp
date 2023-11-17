#include <imgui.h>
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

	for (auto& item : data.GetObjects())
	{
		LOG_INFO(item.first);
		for (auto& item : item.second)
		{
			LOG_INFO(item.name); LOG_INFO(item.value);
		}
	}
	
  	Window win(1280, 720, "Парковка");
    while (win.StartUpdate())
    {
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({(float)win.GetWidth(), (float)win.GetHeight()});
		ImGui::SetWindowPos({0, 0});

		ImGui::SetCursorPos({(float)win.GetWidth()/2, (float)win.GetHeight()/2});
		if (ImGui::Button("Add", {40, 40}))
		{
			
		}
		
		ImGui::End();
		
        win.EndUpdate();
    }

	Window::StopIMGUI();

    return 0;
}
