#pragma once

#include <shelling/mainshell.h>
#include <shelling/parking.h>
#include <shelling/example.h>
#include <shelling/info.h>
#include <shelling/graph.h>
#include <shelling/funcs.h>

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
#if defined(DEMO)
		while (m_Window.StartUpdate())
		{
			ImPlot::ShowUserGuide();
			ImPlot::ShowDemoWindow();

			m_Window.EndUpdate();
		}
#else

		INIT_LOG();

#if defined(_WIN32)
		Shell::ImGuiTalkBuffer::file = std::make_unique<File::Extracter>(L"db.rot");
#else
		Shell::ImGuiTalkBuffer::file = std::make_unique<File::Extracter>("db.rot");
#endif
		Shell::ImGuiTalkBuffer::parser = std::make_unique<File::Parser>(Shell::ImGuiTalkBuffer::file);

		m_Window.GetWindowIO().Fonts->AddFontFromFileTTF("JetBrainsMonoNLNerdFontMono-Regular.ttf", 23, nullptr
			, m_Window.GetWindowIO().Fonts->GetGlyphRangesCyrillic());

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

		std::vector<Shell::Shell*> shells;

		Shell::Parking   win1(true, "DataShow", m_Window.GetWidth(), m_Window.GetHeight(), m_Window);
		Shell::Example   win2(false, "Исходный текст", m_Window.GetWidth(), m_Window.GetHeight(), m_Window);
		Shell::Info      win3(false, "Информация", 500, 500, m_Window);
		Shell::Graphic   win4(false, "График", 500, 1000, m_Window);
		Shell::Functions win5(false, "Функции", 500, 1000, m_Window);

		win1.SetPos(0, 0);
		win2.SetPos(0, 29);
		win3.SetPos(0, 29);

		shells.push_back(&win1);
		shells.push_back(&win3);
		shells.push_back(&win2);
		shells.push_back(&win4);
		shells.push_back(&win5);

		Shell::ImGuiTalkBuffer::windows = &shells;
		Shell::ImGuiTalkBuffer::parser->ReadData(Shell::ImGuiTalkBuffer::data);

		while (m_Window.StartUpdate())
		{
			MyGui::SuicideCommand commando;

			win1.SetScreen(m_Window.GetWindowIO().DisplaySize.x, m_Window.GetWindowIO().DisplaySize.y);
			win2.SetScreen(m_Window.GetWindowIO().DisplaySize.x, m_Window.GetWindowIO().DisplaySize.y - 29);

			for (auto& shell : shells)
				shell->OnUpdate();
			Shell::ImGuiTalkBuffer::parser->ReadData(Shell::ImGuiTalkBuffer::data);
			
			m_Window.EndUpdate();
		}
#endif
		
		Window::StopIMGUI();
	}
	
	
private:
	Window m_Window;
	
};
