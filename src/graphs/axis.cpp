#include <axis.h>

#include <imgui.h>
#include <implot.h>

#include <math_expr.hpp>

namespace Graph {

	Plot::Plot(int min, int max, float precision)
		: m_min(min), m_max(max), m_precision(precision)
	{ }

	void Plot::Update(const std::vector<std::string*>& exprs, std::vector<std::pair<std::vector<double>, std::vector<double>>>* populus)
	{
		const int range = (abs(m_min) + abs(m_max)) * 1/m_precision;		

		if (m_min > m_max) return;
		
		if (ImPlot::BeginPlot("Ось", ImVec2(-1, -1), ImPlotFlags_CanvasOnly)) { // Создание графика
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

    RESULT Plot::Relate(const std::string& expr_str, std::vector<double>& x, std::vector<double>& y) const
	{
		auto points = Utils::do_math(expr_str, m_min, m_max, m_precision, ImGui::GetTime());

		x = points.first;
		y = points.second;

		return 0;
	}
}
