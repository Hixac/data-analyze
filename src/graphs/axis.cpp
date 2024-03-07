#include <axis.h>

#include <cassert>
#include <imgui.h>
#include <implot.h>

#include <mathread.h>

namespace Graph {

	Plot::Plot(int min, int max, float precision)
		: m_min(min), m_max(max), m_precision(precision)
	{ }

	void Plot::Update(const std::vector<std::string*>& exprs, std::vector<std::pair<std::vector<double>, std::vector<double>>>* populus)
	{
		m_expressions = exprs;
		const int range = (abs(m_min) + abs(m_max)) * 1/m_precision;		

		if (m_min > m_max) return;
		
		if (ImPlot::BeginPlot("Ось", ImVec2(-1, 0))) { // Создание графика
			ImPlot::SetupAxes("x","y");
		
			for (std::string* expr : exprs)
			{
				std::vector<double> x, y;
				if (Relate(*expr, x, y) < 0) {
					continue;
				}
				if (populus != nullptr) populus->push_back({x, y});
			
				ImPlot::PlotLine(("f(x) = " + *expr).c_str(), &x[0], &y[0], range);
			}
		
			ImPlot::EndPlot(); // Уничтожение графика
		}
	}

    RESULT Plot::Relate(const std::string& expr, std::vector<double>& x, std::vector<double>& y) const
	{
		for (float i = m_min; i < m_max; i += m_precision) {
			Yard yard(expr, i);

			float answer; int err;
			if ((err = yard.result(answer)) < 0)
			{
				LOG_INFO("YARD ERROR CODE: " + std::to_string(err));
			    return err;
			}
		    
			x.push_back(i);
			y.push_back(answer);
		}

		return 0;
	}
}
