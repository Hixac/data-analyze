#pragma once

#include <shelling/mainshell.h>

#include <window.h>

namespace Shell {

	class Example : public Shell
	{
	public:
	    Example(bool show, const std::string label, unsigned int width, unsigned int height, Window& window) : m_Window(&window), Shell(label, width, height, show) { }

		void OnUpdate() override;

	private:
		void UpdateWrap() override;
		
	    Window* m_Window;
	};
			
}
