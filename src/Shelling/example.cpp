#include <Shelling/example.h>

#include <filedialog.hpp>
#include <misc/cpp/imgui_stdlib.h>

#include <fstream>

namespace Shell {

	static std::wstring s2ws(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

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
					auto ws = s2ws(filepath.out);
					File::Extracter* pfile = new File::Extracter(ws);
					ImGuiTalkBuffer::file.reset(pfile);
				}
			}
			ImGui::Separator();
			if (ImGui::Button("Сохранить файл"))
			{
				auto filepath = Utils::FileDialog::Get().Save({"*", "rot"});
				if (filepath.err == Utils::FileDialog::None) {
					std::ofstream out(s2ws(filepath.out));
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
