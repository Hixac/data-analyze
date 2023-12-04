#pragma once

#include <Shelling/mainshell.h>

#include <window.h>

namespace Shell {

	class Example : public Shell
	{
	public:
	    Example(Window& window);

		void OnUpdate() override;

	private:
	    Window* m_Window;
	};
			
}
