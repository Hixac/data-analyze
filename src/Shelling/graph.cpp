#include "dataunit.h"
#include <Shelling/graph.h>

#include <implot.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <mathread.h>

namespace Shell {

	Graph::Graph(bool show, const std::string label, unsigned int width, unsigned int height, Window& window)
	: m_Window(&window), Shell(label, width, height, show)
	{
		ImGui::SetNextWindowSize({(float)m_width, (float)m_height});
	}
	
	void Graph::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Graph::UpdateWrap()
	{
		ImGui::Begin(m_label.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginTabBar("#Plots")) {
			if (ImGui::BeginTabItem("Ось"))
			{
				Axis();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Рассеянность")) {
				Scatter();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Гистограмма")) {
			    Histogram();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		
		ImGui::End();
	}

	void Graph::Histogram()
	{
		static int selected_object = 0;
		
		if (selected_object > ImGuiTalkBuffer::data.GetObjects().size())
			selected_object = 0;
		
		if (ImGui::BeginPopup("ListingObjects"))
        {
            ImGui::SeparatorText("Таблицы");
            for (int i = 0; i < ImGuiTalkBuffer::data.GetObjects().size(); i++)
                if (ImGui::Selectable(ImGuiTalkBuffer::data.GetObjects()[i].first.c_str()))
                    selected_object = i;
            ImGui::EndPopup();
        }

		if (ImGui::Button("Выбрать таблицу")) ImGui::OpenPopup("ListingObjects");
		ImGui::SameLine();
		ImGui::Button("?");
        ImGui::SetItemTooltip("График рассеянности не требует конкретных таблиц сгенерированные при помощи оси.\nЧтобы самому заполнить точки:\n необходимо создать таблицу и самостоятельно заполнить таблицу переменными\n как тип 'point', по шаблону 'x;y' (без апострофов)\n где 'икс' и 'игрек' является вещественным либо целочисленным числом\n а затем выбрать эту таблицу, дабы она отобразилась на графике.");
		// Бог терпел и нам велел.

		if (ImPlot::BeginPlot("##Histograms")) {
			Database::Object obj = ImGuiTalkBuffer::data.GetObjects()[selected_object];
			auto points = GetPoints(obj);
			if (points.first.size() > 0)
				ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);

			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
			ImPlot::PlotHistogram("##Данные", &points.second[0], points.second.size());
			ImPlot::EndPlot();
		}
	}

	std::pair<std::vector<float>, std::vector<float>> Graph::GetPoints(Database::Object obj)
	{
		std::vector<float> x, y;
		
		for (int i = 0; i < obj.second.size(); i++)
		{
			auto& unit = obj.second[i];

			if (unit.err != Database::Error::None) continue;
			if (unit.type == Database::Type::Point)
			{
				std::pair<float, float> pair;
				if (unit.GetPointers(pair))
				{
					x.push_back(pair.first);
					y.push_back(pair.second);						
				}
			}
			else if (unit.type == Database::Type::Integer || unit.type == Database::Type::Float)
			{
				x.push_back(i - 25);
				y.push_back(std::stof(unit.value));					
			}
		}

		return {x, y};
	}
	
	void Graph::Scatter()
	{
		static int selected_object = 0;
		
		if (selected_object > ImGuiTalkBuffer::data.GetObjects().size())
			selected_object = 0;
		
		if (ImGui::BeginPopup("ListingObjects"))
        {
            ImGui::SeparatorText("Таблицы");
            for (int i = 0; i < ImGuiTalkBuffer::data.GetObjects().size(); i++)
                if (ImGui::Selectable(ImGuiTalkBuffer::data.GetObjects()[i].first.c_str()))
                    selected_object = i;
            ImGui::EndPopup();
        }

		if (ImGui::Button("Выбрать таблицу")) ImGui::OpenPopup("ListingObjects");
		ImGui::SameLine();
		ImGui::Button("?");
        ImGui::SetItemTooltip("График рассеянности не требует конкретных таблиц сгенерированные при помощи оси.\nЧтобы самому заполнить точки:\n необходимо создать таблицу и самостоятельно заполнить таблицу переменными\n как тип 'point', по шаблону 'x;y' (без апострофов)\n где 'икс' и 'игрек' является вещественным либо целочисленным числом\n а затем выбрать эту таблицу, дабы она отобразилась на графике.");
		// Бог терпел и нам велел.
		
		if (ImPlot::BeginPlot("##Рассеяность")) {
			
			auto& obj = ImGuiTalkBuffer::data.GetObjects()[selected_object];
			auto points = GetPoints(obj);
			if (points.first.size() > 0)
				ImPlot::PlotScatter(("##" + obj.first).c_str(), &points.first[0], &points.second[0], points.first.size());

			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}
	}
	
	void Graph::Axis()
	{
		static std::string expr = "cos(sin(x))";

		static int selected_object = -1;
		
		if (selected_object > ImGuiTalkBuffer::data.GetObjects().size()) // Чтобы не было ошибки, при удалении нужной таблицы (ДДАААА МНЕ ЛЕНЬ ДЕЛАТЬ НОМРАЛНЬО)
			selected_object = -1;
		
		if (ImGui::BeginPopup("ListingObjects"))
        {
            ImGui::SeparatorText("Таблицы");
            for (int i = 0; i < ImGuiTalkBuffer::data.GetObjects().size(); i++)
                if (ImGui::Selectable(ImGuiTalkBuffer::data.GetObjects()[i].first.c_str()))
                    selected_object = i;
			if (ImGui::Selectable("Отключить"))
				selected_object = -1;
            ImGui::EndPopup();
        }	
		
		if (selected_object != -1)
		{
			auto& expressions = ImGuiTalkBuffer::data.GetObjects()[selected_object].second;		
			for (int i = 0; i < expressions.size(); i++)
			{
				std::string label = expressions[i].name;
				ImGui::Text("%s", label.c_str());
				ImGui::SameLine();
				ImGui::InputText(("##" + label).c_str(), &expressions[i].value);
			}
			ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
		}
		else
		{
			ImGui::Text("Функция");
			ImGui::SameLine();
			ImGui::InputText("##math expression", &expr);
		}

		ImGui::SameLine();
		if (ImGui::Button("+"))
			ImGui::OpenPopup("ListingObjects");
		ImGui::SameLine();
		ImGui::Button("?");
        ImGui::SetItemTooltip("Поддерживающиеся символы: + (сумма), - (разность), * (умножение), / (деление), ^ (степень)\nКлючевые слова: time (всего пройденное время), dtime (изменение времени)\nФункции: sin([выражение]), cos([выражение]), abs([выражение])");	
		
		if (selected_object == -1)
		{
			double x[51], y[51];
			for (int i = 0; i < 51; i++)
			{
				Yard yard(expr, i - 25);
				float answer; int err;
				if ((err = yard.result(answer)) < 0)
				{
					LOG_INFO("YARD ERROR CODE: " + std::to_string(err));
					break;
				}
				x[i] = i - 25;
				y[i] = answer;
			}

			if (ImPlot::BeginPlot("##Ось", ImVec2(-1,0)))
			{
				ImPlot::PlotLine(("f(x) = " + expr).c_str(), x, y, 51);
				ImPlot::EndPlot();
			}

			if (ImGui::Button("Сгенерировать данные"))
			{
				std::vector<Database::Dataunit> units;
				for (int i = 0; i < 51; i++)
				{
					units.push_back({"f(" + std::to_string(x[i]) + ")", std::to_string(x[i]) + ";" + std::to_string(y[i]), Database::Type::Point, Database::Error::None});
				}
				ImGuiTalkBuffer::data.Add("Таблица", units);
				ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
			}
		}
		else
		{
			std::vector<std::pair<std::vector<double>, std::vector<double>>> datas;
			auto& expressions = ImGuiTalkBuffer::data.GetObjects()[selected_object].second;		
			for (int i = 0; i < expressions.size(); i++)
			{
				std::pair<std::vector<double>, std::vector<double>> coords;
				for (int j = 0; j < 51; j++)
				{
					Yard yard(expressions[i].value, j - 25);
					float answer; int err;
					if ((err = yard.result(answer)) < 0)
					{
						LOG_INFO("YARD ERROR CODE: " + std::to_string(err));
						break;
					}
					coords.first.push_back(j - 25);
					coords.second.push_back(answer);
				}
				datas.push_back(coords);
				coords.first.clear(); coords.second.clear();
			}
			
			if (ImPlot::BeginPlot("Ось", ImVec2(-1,0)))
			{
				for (int i = 0; i < expressions.size(); i++)
				{
					if (datas[i].first.empty()) continue;
					ImPlot::PlotLine(("f(x) = " + expressions[i].value).c_str(), &datas[i].first[0], &datas[i].second[0], 51);
				}
				ImPlot::EndPlot();
			}

			if (ImGui::Button("Сгенерировать данные"))
			{
				for (int i = 0; i < datas.size(); i++)
				{
					std::vector<Database::Dataunit> units;
					for (int j = 0; j < datas[i].first.size(); j++)
						units.push_back({"f(" + std::to_string(datas[i].first[j]) + ")", std::to_string(datas[i].first[j]) + ";" + std::to_string(datas[i].second[j]), Database::Type::Point, Database::Error::None});
					ImGuiTalkBuffer::data.Add("Таблица " + std::to_string(i + 1), units);
				}
				ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
			}
		}
	}
	
}
