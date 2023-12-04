#include <Shelling/info.h>

#include <imgui/deltatime.h>

namespace Shell {

	Info::Info(Window& window)
		: m_Window(&window)
	{}

	void Info::OnUpdate()
	{
		ImGui::Begin("Check", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({m_Window->GetWindowIO().DisplaySize.x, ImGuiTalkBuffer::winCheckSize });
		ImGui::SetWindowPos({0, m_Window->GetWindowIO().DisplaySize.y - ImGuiTalkBuffer::winCheckSize });
				
		ImGui::SameLine();		
		ImGui::Text("dtime: %f", MyGui::BasicInformation::deltatime);
		
		ImGui::End();
	}
	
}
