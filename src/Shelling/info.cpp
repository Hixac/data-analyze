#include <Shelling/info.h>

#include <imgui/deltatime.h>

namespace Shell {

	void Info::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Info::UpdateWrap()
	{
		ImGui::SetNextWindowBgAlpha(1);
		ImGui::Begin(m_label.c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({(float)m_width, (float)m_height});
		ImGui::SetWindowPos({m_posx, m_posy});
		
		ImGui::SameLine();		
		ImGui::Text("Разность времени появления и исчезновения кадра: %f", MyGui::BasicInformation::deltatime);
		
		ImGui::End();
	}
	
}
