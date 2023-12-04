#include <Shelling/example.h>

#include <misc/cpp/imgui_stdlib.h>

namespace Shell {

	Example::Example(Window& window)
		: m_Window(&window)
	{ }
	
	void Example::OnUpdate()
	{
		ImGui::Begin("Example", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({m_Window->GetWindowIO().DisplaySize.x / 3, m_Window->GetWindowIO().DisplaySize.y - ImGuiTalkBuffer::winCheckSize});
		ImGui::SetWindowPos({m_Window->GetWindowIO().DisplaySize.x - m_Window->GetWindowIO().DisplaySize.x / 3, 0});

		std::string text = ImGuiTalkBuffer::file->GetContent();
		ImGui::InputTextMultiline("##Example", &text, {m_Window->GetWindowIO().DisplaySize.x / 2, m_Window->GetWindowIO().DisplaySize.y - 100});
	    ImGuiTalkBuffer::file->SetContent(text);

		ImGuiTalkBuffer::data = ImGuiTalkBuffer::parser->Process();
		
		ImGui::End();

	}
	
}
