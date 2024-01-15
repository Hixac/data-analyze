#pragma once

#include <Shelling/mainshell.h>

#include <window.h>
#include <dataunit.h>

#include <vector>

namespace Shell {

	class Graph : public Shell
	{
	public:
	    Graph(bool show, const std::string label, unsigned int width, unsigned int height, Window& window);

		void OnUpdate() override;
	private:
		void UpdateWrap() override;

		void Axis();
		void Scatter();
		void Histogram();

		std::pair<std::vector<float>, std::vector<float>> GetPoints(Database::Object obj);
		
		Window* m_Window;
	};
	
}
