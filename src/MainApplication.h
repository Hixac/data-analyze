#pragma once

#include <Shelling/mainshell.h>
#include <Shelling/parking.h>
#include <Shelling/example.h>
#include <Shelling/info.h>

#include <Application.h>

#define DEBUG
#include <Base.h>

#include <imgui/deltatime.h>

class MainApplication : public Application
{
public:
	MainApplication(unsigned int width, unsigned int height, const char* title)
		: m_Window(width, height, title) { }
	~MainApplication() = default;

	void Run() override
	{
		INIT_LOG();

		Shell::ImGuiTalkBuffer::file = std::make_unique<File::Extracter>("db.rot");
		Shell::ImGuiTalkBuffer::parser = std::make_unique<File::Parser>(*Shell::ImGuiTalkBuffer::file);
		
	    m_Window.GetWindowIO().Fonts->AddFontFromFileTTF("../res/JetBrainsMonoNLNerdFontMono-Regular.ttf", 23);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

		std::vector<Shell::Shell*> shells;
		
		Shell::Parking win1(m_Window);
		Shell::Example win2(m_Window);
		Shell::Info win3(m_Window);

		shells.push_back(&win1);
		shells.push_back(&win2);
		shells.push_back(&win3);
			
		while (m_Window.StartUpdate())
		{
			MyGui::SuicideCommand commando;

			for (auto& shell : shells)
				shell->OnUpdate();
			
			m_Window.EndUpdate();
		}

		Window::StopIMGUI();
	}
	
	
private:
	Window m_Window;
	
};
