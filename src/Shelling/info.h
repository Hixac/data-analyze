#pragma once

#include <Shelling/mainshell.h>

#include <window.h>

namespace Shell {

	class Info : public Shell
	{
	public:
	    Info(bool show, const std::string label, unsigned int width, unsigned int height, Window& window) : m_Window(&window), Shell(label, width, height, show) { }

		void OnUpdate() override;
	private:
		void UpdateWrap() override;
		
		Window* m_Window;
	};
	
}
