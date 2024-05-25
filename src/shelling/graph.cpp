#include <cstdint>
#include <shelling/graph.h>

#include <random>
#include <algorithm>
#include <numeric>

#include <implot.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <window.h>
#include <axis.h>
#include <math_expr.hpp>

namespace Shell {

	Graphic::Graphic(bool show, const std::string label, unsigned int width, unsigned int height, Window& window)
	: m_Window(&window), Shell(label, width, height, show)
	{
		ImGui::SetNextWindowSize({(float)m_width, (float)m_height});
	}
	
	void Graphic::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Graphic::UpdateWrap()
	{
		ImGui::Begin(m_label.c_str());
		
		if (ImGui::BeginTabBar("#Plots")) {
			if (ImGui::BeginTabItem("Ось")) {
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
			if (ImGui::BeginTabItem("Распределение Фишера")) {
				FisherDistribution();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Нормальное распределение")) {
				Gauss();
				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}

		static bool full_screen = false;
		
		float win_x = ImGui::GetWindowSize().x;
		
		ImGui::SetCursorPos({ win_x - 180, 34 });
		ImGui::Text("Полный экран");

		ImGui::SetCursorPos({ win_x - 50, 32.5 });
		if (ImGui::Button("|+|")) {

			static ImVec2 oldpos;
			static ImVec2 oldsize;
			
			full_screen = !full_screen;

			if (full_screen == true) {
			    oldpos = ImGui::GetWindowPos();
			    oldsize = ImGui::GetWindowSize();	
			}			
			
			if (full_screen == false) {
				ImGui::SetWindowPos(oldpos);
				ImGui::SetWindowSize(oldsize);	
			}
		}
		
		if (full_screen == true) {
			ImGui::SetWindowPos({0, 0});
			ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
		}
		
		ImGui::End();
	}

	void Graphic::Gauss()
	{
		static int min = -100;
		static int max = 100;

		static float variance = 10;
	    static float median = 0;

		ImGui::InputInt("Мин", &min);
		ImGui::InputInt("Макс", &max);

		ImGui::SliderFloat("Дисперсия", &variance, 0.001, 100);
		ImGui::SliderFloat("Медиана", &median, min, max);

	    min = std::clamp(min, INT_MIN, max - 1);
		variance = std::clamp<float>(variance, 0.01, 100);
	    median = std::clamp<float>(median, INT_MIN, 1000000);
		
		auto points = Utils::do_math("(1/(a*sqrt(2*3.1415926))) * 2.71828182 ^ (-0.5 * ((x - b)/a) ^ 2)", min, max, 0.01, 0, variance, median);

		if (ImPlot::BeginPlot("##Pppp", {-1, -1})) {

			std::string func = "1/(a*sqrt(2*pi)) * e ^ (-0.5 * ((x - b)/a) ^ 2)";
			ImPlot::PlotLine(func.c_str(), &points.first[0], &points.second[0], points.first.size());

			ImPlot::EndPlot();
		}
	}
	
	void Graphic::FisherDistribution()
	{
		static int rolls = 1000;
		static int limit = 100;

		static int numerator = 2;
		static int denominator = 2;

		static bool hide_all = true;
		
		ImGui::InputInt("Количество экспериментов", &rolls);
		if (!hide_all) ImGui::InputInt("Ограничение", &limit);

		ImGui::InputInt("Степень свободы (числитель)", &numerator);
		ImGui::InputInt("Степень свободы (знаменатель)", &denominator);
		
		// restricting values
		rolls = std::clamp(rolls, 1, 1000000);
		limit = std::clamp(limit, 1, 1000000);
	    numerator = std::clamp(numerator, 1, 1000000);
	    denominator = std::clamp(denominator, 1, 1000000);
		
		std::default_random_engine generator;
		std::fisher_f_distribution<> distribution(numerator, denominator);

		if (ImGui::Button("Осциллографический вид")) hide_all = !hide_all;
		
		if (hide_all) {
		
			static int count = 10;
			
			ImGui::InputInt("Количество чисел", &count);
		
			count = std::clamp(count, 1, 10000000);

			double* counter = (double*)calloc(count, sizeof(double));
			for (int i = 0; i < rolls; ++i) {
				double number = distribution(generator);
				if (number > 0 && number < count) ++counter[int(number)];
			}

			double* sequence = (double*)calloc(count, sizeof(double));
			std::iota(sequence, sequence + count, 0);
			
			if (ImPlot::BeginPlot("F-распределение", ImVec2(-1, -1))) {
				ImPlot::PlotLine("##ебись оно конём", sequence, counter, count);
			
				ImPlot::EndPlot();
			}

			return;
		}

		std::vector<double> nums;
		for (int i = 0; i < rolls; ++i) {
			double number = distribution(generator);
			if (number > 0 && number < limit) nums.push_back(number);
		}

		double* sequence = (double*)calloc(nums.size(), sizeof(double));
		std::iota(sequence, sequence + nums.size() - 1, 0);
		
	    if (ImPlot::BeginPlot("F-распределение", ImVec2(-1, -1))) {
			ImPlot::PlotLine("##ебись оно конём", sequence, &nums[0], nums.size());
			
			ImPlot::EndPlot();
		}
	}
	
	void Graphic::Histogram()
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
        ImGui::SetItemTooltip("Гистограмма визуально представляет распределение непрерывной числовой переменной,\n при котором измеряется частота появления в наборе данных сходных значений.\n По оси х откладываются числовые значения, которые разбиты на диапазоны или интервалы.");
		// Бог терпел и нам велел.

		if (ImPlot::BeginPlot("##Histograms", ImVec2(-1, -1))) {
			Database::Object obj = ImGuiTalkBuffer::data.GetObjects()[selected_object];
			auto points = GetPoints(obj);
			if (points.first.size() > 0)
				ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);

			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
			ImPlot::PlotHistogram("##Данные", &points.second[0], points.second.size());
			ImPlot::EndPlot();
		}
	}

	std::pair<std::vector<float>, std::vector<float>> Graphic::GetPoints(Database::Object obj)
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
	
	void Graphic::Scatter()
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
		
		if (ImPlot::BeginPlot("##Рассеяность", ImVec2(-1, -1))) {
			
			auto& obj = ImGuiTalkBuffer::data.GetObjects()[selected_object];
			auto points = GetPoints(obj);
			if (points.first.size() > 0)
				ImPlot::PlotScatter(("##" + obj.first).c_str(), &points.first[0], &points.second[0], points.first.size());

			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}
	}
	
	void Graphic::Axis()
	{
		static bool hide_all = false;
		
		ImVec2 oldpos = ImGui::GetCursorPos();
		float win_x = ImGui::GetWindowSize().x;
		
		ImGui::SetCursorPos({ win_x - 300, 34 });
		ImGui::Text("График");

		ImGui::SetCursorPos({ win_x - 230, 32.5 });
		if (ImGui::Button("|-|")) {
			hide_all = !hide_all;
		}
		ImGui::SetCursorPos(oldpos);

		static int min = -200;
		static int max = 200;
		static float precision = 1;

		static std::vector<std::string*> exprs;
	    auto populus = new std::vector<std::pair<std::vector<double>, std::vector<double>>>;

		static int selected_object = -1;
		static std::vector<std::string> def_exprs = { "sin(x)", "x", "x", "x", "x", "x", "x", "x", "x", "x"};
		static int funcs = 1;

		if (selected_object >= ImGuiTalkBuffer::data.GetObjects().size()) {
			selected_object = -1;
		}

		exprs.clear();
		for (int i = 0; i < funcs; ++i) {
			exprs.push_back(&def_exprs[i]);
		}
		
		if (selected_object != -1) {
			for (auto& unit : ImGuiTalkBuffer::data.GetObjects()[selected_object].second)
				exprs.push_back(&unit.value);
		}

		Graph::Plot plot(min, max, 1 / precision);
		plot.Update(exprs, populus, hide_all);
		if (hide_all) return;


		ImGui::PushItemWidth(500);

		ImGui::SliderInt("Кол-во функций", &funcs, 0, 10);
		
		
		if (ImGui::BeginPopup("ListingObjects")) {
			ImGui::SeparatorText("Таблицы");
			for (int i = 0; i < ImGuiTalkBuffer::data.GetObjects().size(); i++)
				if (ImGui::Selectable(ImGuiTalkBuffer::data.GetObjects()[i].first.c_str()))
					selected_object = i;
			if (ImGui::Button("Отключить"))
				selected_object = -1;

            ImGui::EndPopup();
        }

		for (int i = 0; i < exprs.size(); ++i) {
			auto expr = exprs[i];
			ImGui::PushID(i);
			ImGui::InputText("##Function", expr);
			ImGui::PopID();
		}
		ImGui::SameLine();
		ImGui::Button("?");
		ImGui::SetItemTooltip("Поддерживающиеся символы: + (сумма), - (разность (только в инфиксной форме, для отрицания \"(0-x)\") )\n* (умножение), / (деление), ^ (степень)\nКлючевые слова: time (всего пройденное время)\nФункции: cos, sin, tan (точки соединены между собой всегда, потому асимптот не видно)\nacos, asin, cosh, sinh, acosh, asinh");

		// Записывает все данные внутрь, потому лучше оставить
		ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
	    
		ImGui::SameLine();
		if (ImGui::Button("+")) ImGui::OpenPopup("ListingObjects");

		ImGui::InputInt("от", &min);
		ImGui::InputInt("до", &max);
		ImGui::SliderFloat("множитель точек", &precision, 1, 100);
		ImGui::PopItemWidth();

		min = std::clamp(min, INT_MIN, max - 1);

		ImGui::SameLine();
		ImGui::Button("?");
        ImGui::SetItemTooltip("Большое значение приведёт к потере производительности");

		if (ImGui::Button("Сгенерировать данные")) {
			for (int i = 0; i < populus->size(); ++i) {
				std::vector<Database::Dataunit> units;
				for (int j = 0, g = 0; j < (*populus)[i].first.size(); j += int((*populus)[i].first.size() / 100)) {
					auto points = (*populus)[i];

					Database::Dataunit unit = {
						.name = "f(" + std::to_string(points.first[j]) + ")",
						.value = std::to_string(points.first[j]) + ";" + std::to_string(points.second[j]),
						.type = Database::Type::Point,
						.err = Database::Error::None,
					};
					units.push_back(unit);
				}

				ImGuiTalkBuffer::data.Add(*exprs[i], units);
			}

			ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
		}

		ImGui::Text("Информация о функциях");

		size_t size = populus->size();
		for (int i = 0; i < size; ++i) {
			
			auto ys = (*populus)[i].second;
				  
		    double maximum = INT32_MIN;
		    double minimum = INT32_MAX;
			
			for (int j = 0; j < (*populus)[i].second.size(); ++j) {
			    double y = ys[j];

				if (y > maximum) maximum = y;
				if (y < minimum) minimum = y;
			}

			ImGui::Text((std::to_string(i + 1) + ". мин: " + std::to_string(minimum) + "; макс: " + std::to_string(maximum)).c_str());
		}

		ImGui::Text("Нули функции");
		ImGui::SameLine();
		ImGui::Button("?");
	    ImGui::SetItemTooltip("'Иксы' указаны лишь приблизительно, проверяется когда первая точка ниже оси абсции,\nа следующая выше, либо по максимуму и минимуму.\nТакже причина неверно указанного 'икса' является отличное от нуля значение в типе double.\nДабы это проследить, можете максимально приблизить точку на графике, чтобы удостовериться,\nчто точка не соприкасается.");
		
		for (int i = 0; i < size; ++i) {
			
			auto xs = (*populus)[i].first;
			auto ys = (*populus)[i].second;
			
			for (int j = 0; j < (*populus)[i].first.size() - 1; ++j) {
			    if (ys[j] <= 0 && ys[j + 1] >= 0) {
					ImGui::Text((std::to_string(i + 1) + ". x: " + std::to_string(xs[j] < xs[j+1] ? xs[j] : xs[j+1])).c_str());
				}
			}

		}
	}
	
}
