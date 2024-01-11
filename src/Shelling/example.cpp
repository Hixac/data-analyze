#include <Shelling/example.h>

#include <misc/cpp/imgui_stdlib.h>

namespace Shell {

	void Example::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Example::UpdateWrap()
	{
		ImGui::SetNextWindowBgAlpha(1);
		ImGui::Begin("Example", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({(float)m_width, (float)m_height});
		ImGui::SetWindowPos({m_posx, m_posy});

		std::string text = ImGuiTalkBuffer::file->GetContent();
		ImGui::InputTextMultiline("##Example", &text, {m_Window->GetWindowIO().DisplaySize.x / 2, m_Window->GetWindowIO().DisplaySize.y - 100});
	    ImGuiTalkBuffer::file->SetContent(text);
		
		ImGui::End();

	}
	
}
