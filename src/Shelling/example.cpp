#include <Shelling/example.h>

#include <filedialog.hpp>
#include <misc/cpp/imgui_stdlib.h>

#include <fstream>

namespace Shell {

	void Example::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Example::UpdateWrap()
	{
		ImGui::SetNextWindowBgAlpha(1);
		ImGui::Begin(m_label.c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
					 | ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize({(float)m_width, (float)m_height});
		ImGui::SetWindowPos({m_posx, m_posy});

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Открыть файл"))
			{
				auto filepath = Utils::FileDialog::Get().Open({"База данных", "rot"});
				if (filepath.err == Utils::FileDialog::None) {
					File::Extracter* pfile = new File::Extracter(filepath.out);
					ImGuiTalkBuffer::file.reset(pfile);
				}
			}
			ImGui::Separator();
			if (ImGui::Button("Сохранить файл"))
			{
				auto filepath = Utils::FileDialog::Get().Save({"*", "rot"});
				if (filepath.err == Utils::FileDialog::None) {
					std::ofstream out(filepath.out);
					out << ImGuiTalkBuffer::file->GetContent();
				}
			}
			ImGui::Separator();
			ImGui::EndMenuBar();
		}
		
		std::string text = ImGuiTalkBuffer::file->GetContent();
		ImGui::InputTextMultiline("##Example", &text, {(float)m_width, (float)m_height - 16});
	    ImGuiTalkBuffer::file->SetContent(text);
		
		ImGui::End();

	}
	
}
