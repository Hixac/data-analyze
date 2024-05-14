#include <axis.h>

#include <imgui.h>
#include <implot.h>

#include <math_expr.hpp>

namespace Graph {

	Plot::Plot(int min, int max, float precision)
		: m_min(min), m_max(max), m_precision(precision)
	{ }

	void Plot::Update(const std::vector<std::string*>& exprs, std::vector<std::pair<std::vector<double>, std::vector<double>>>* populus,
		bool show)
	{
		const int range = (abs(m_min) + abs(m_max)) * 1/m_precision;

		if (m_min > m_max) return;

		for (std::string* expr : exprs)
		{
			std::vector<double> x, y;
			if (Relate(*expr, x, y) < 0) {
				continue;
			}
			if (populus != nullptr) populus->push_back({x, y});
		}
		
		if (show && ImPlot::BeginPlot("Ось", ImVec2(-1, -1), ImPlotFlags_Equal)) { // Создание графика
			ImPlot::SetupAxes("x","y");
		
			for (int i = 0; i < populus->size(); ++i)
			{
				auto pop = (*populus)[i];
				ImPlot::PlotLine(("f(x) = " + *exprs[i]).c_str(), &pop.first[0], &pop.second[0], range);
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
