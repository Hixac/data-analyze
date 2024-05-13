#include <Shelling/graph.h>

#include <implot.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <axis.h>

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
        ImGui::SetItemTooltip("Гистограмма показывает величину разных точек.");
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

		ImGui::SetCursorPos({ win_x - 120, 33 });
		ImGui::Text("График");

		ImGui::SetCursorPos({ win_x - 50, 32.5 });
		if (ImGui::Button("|+|")) {
			hide_all = !hide_all;
		}
		ImGui::SetCursorPos(oldpos);

		static int min = -200;
		static int max = 200;
		static float precision = 1;

		static std::vector<std::string*> exprs;
		auto populus = new std::vector<std::pair<std::vector<double>, std::vector<double>>>;

		if (hide_all) {

			Graph::Plot plot(min, max, precision);
			plot.Update(exprs, populus);

			return;
		}

		static int selected_object = -1;

		ImGui::PushItemWidth(500);

		static std::vector<std::string> def_exprs = { "sin(x)", "x", "x", "x", "x", "x", "x", "x", "x", "x"};
		static int funcs = 1;
		ImGui::SliderInt("Кол-во функций", &funcs, 0, 10);
		
		if (selected_object > ImGuiTalkBuffer::data.GetObjects().size()) {
			selected_object = -1;
		}
		
		if (ImGui::BeginPopup("ListingObjects"))
		{
			ImGui::SeparatorText("Таблицы");
			for (int i = 0; i < ImGuiTalkBuffer::data.GetObjects().size(); i++)
				if (ImGui::Selectable(ImGuiTalkBuffer::data.GetObjects()[i].first.c_str()))
					selected_object = i;
			if (ImGui::Button("Отключить"))
				selected_object = -1;

            ImGui::EndPopup();
        }

		exprs.clear();
		for (int i = 0; i < funcs; ++i) {
			exprs.push_back(&def_exprs[i]);
		}
		
		if (selected_object != -1) {
			for (auto& unit : ImGuiTalkBuffer::data.GetObjects()[selected_object].second)
				exprs.push_back(&unit.value);
		}
		

		for (int i = 0; i < exprs.size(); ++i) {
			auto expr = exprs[i];
			ImGui::PushID(i);
			ImGui::InputText("##Function", expr);
			ImGui::PopID();
		}
		// Записывает все данные внутрь, потому лучше оставить
		ImGuiTalkBuffer::parser->WriteData(ImGuiTalkBuffer::data);
	    
		ImGui::SameLine();
		if (ImGui::Button("+")) ImGui::OpenPopup("ListingObjects");

		ImGui::InputInt("от", &min);
		ImGui::InputInt("до", &max);
		ImGui::SliderFloat("точность", &precision, 0.01, 1);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Button("?");
        ImGui::SetItemTooltip("Большие значения \"от\" и \"до\" приведут к уменьшению производительности");

		if (ImGui::Button("Сгенерировать данные") && populus->size() > 0) {
			for (int i = 0; i < populus->size(); ++i) {
				std::vector<Database::Dataunit> units;
				for (int j = 0; j < (*populus)[i].first.size(); ++j) {
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
	}
	
}
