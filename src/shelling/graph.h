#pragma once

#include <shelling/mainshell.h>

#include <window.h>
#include <dataunit.h>

#include <vector>

namespace Shell {

	class Graphic : public Shell
	{
	public:
	    Graphic(bool show, const std::string label, unsigned int width, unsigned int height, Window& window);

		void OnUpdate() override;
	private:
		void UpdateWrap() override;

		void Axis();
		void Gauss();
		void Scatter();
		void Histogram();
		void FisherDistribution();
		
		std::pair<std::vector<float>, std::vector<float>> GetPoints(Database::Object obj);
		
		Window* m_Window;
	};
	
}
