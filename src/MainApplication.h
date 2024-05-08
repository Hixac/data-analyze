#pragma once

#include <Shelling/mainshell.h>
#include <Shelling/parking.h>
#include <Shelling/example.h>
#include <Shelling/info.h>
#include <Shelling/graph.h>

#include <Application.h>

#include <Base.h>

#include <imgui/deltatime.h>

#include <implot.h>
#include <memory>

class MainApplication : public Application
{
public:
	MainApplication(unsigned int width, unsigned int height, const char* title)
		: m_Window(width, height, title) { }
	~MainApplication() = default;

	void Run() override
	{
		INIT_LOG();

		Shell::ImGuiTalkBuffer::file = std::make_unique<File::Extracter>(L"db.rot");
		Shell::ImGuiTalkBuffer::parser = std::make_unique<File::Parser>(Shell::ImGuiTalkBuffer::file);

		m_Window.GetWindowIO().Fonts->AddFontFromFileTTF("JetBrainsMonoNLNerdFontMono-Regular.ttf", 23, nullptr
														 , m_Window.GetWindowIO().Fonts->GetGlyphRangesCyrillic());

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		
		std::vector<Shell::Shell*> shells;
		
		Shell::Parking win1(true, "DataShow", m_Window.GetWidth(), m_Window.GetHeight(), m_Window);
		Shell::Example win2(false, "Исходный текст", m_Window.GetWidth(), m_Window.GetHeight(), m_Window);
		Shell::Info    win3(false, "Информация", m_Window.GetWidth(), m_Window.GetHeight(), m_Window);
		Shell::Graphic win4(false, "График", 500, 1000, m_Window);

		win1.SetPos(0, 0);
		win2.SetPos(0, 29);
		win3.SetPos(0, 29);
		
		shells.push_back(&win1);
		shells.push_back(&win3);
		shells.push_back(&win2);
		shells.push_back(&win4);	

		Shell::ImGuiTalkBuffer::windows = &shells;
		Shell::ImGuiTalkBuffer::parser->ReadData(Shell::ImGuiTalkBuffer::data);
		while (m_Window.StartUpdate())
		{
			MyGui::SuicideCommand commando;

			win1.SetScreen(m_Window.GetWindowIO().DisplaySize.x, m_Window.GetWindowIO().DisplaySize.y);
			win2.SetScreen(m_Window.GetWindowIO().DisplaySize.x, m_Window.GetWindowIO().DisplaySize.y - 29);
			win3.SetScreen(m_Window.GetWindowIO().DisplaySize.x, m_Window.GetWindowIO().DisplaySize.y - 29);

#if 0
			ImPlot::ShowUserGuide();
			ImPlot::ShowDemoWindow();
#else
			for (auto& shell : shells)
				shell->OnUpdate();
			Shell::ImGuiTalkBuffer::parser->ReadData(Shell::ImGuiTalkBuffer::data);
#endif
			
			m_Window.EndUpdate();
		}
		
		Window::StopIMGUI();
	}
	
	
private:
	Window m_Window;
	
};
