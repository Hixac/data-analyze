#pragma once

#include <Shelling/mainshell.h>

#include <window.h>

namespace Shell {

	class Info : public Shell
	{
	public:
		Info(Window& window);

		void OnUpdate() override;
	private:
		Window* m_Window;
	};
	
}
