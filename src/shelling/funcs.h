#pragma once

#include <shelling/mainshell.h>

#include <window.h>

namespace Shell {

	class Functions : public Shell
	{
	public:
	    Functions(bool show, const std::string label, unsigned int width, unsigned int height, Window& window);

		void OnUpdate() override;
	private:
		void UpdateWrap() override;

		void Chance();
		
		Window* m_Window;
	};
	
}
