#include <shelling/funcs.h>

#include <imgui/fastcombo.h>
#include <imgui/dynamictimer.h>

#include <algorithm>

namespace Shell {

	Functions::Functions(bool show, const std::string label, unsigned int width, unsigned int height, Window& window)
	: m_Window(&window), Shell(label, width, height, show)
	{
		ImGui::SetNextWindowSize({(float)m_width, (float)m_height});
	}
	
	void Functions::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Functions::UpdateWrap()
	{
		ImGui::Begin(m_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::BeginTabBar("#Functions")) {
			if (ImGui::BeginTabItem("Вероятность"))
			{
			    Chance();
				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}
		
		ImGui::End();
	}

	void Functions::Chance()
	{
		static int selected_object = -1;
		
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

		if (ImGui::BeginPopup("ErrMsg"))
		{
			ImGui::Text("ОШИБКА: Сумма вероятностей не равно единице");
			
            ImGui::EndPopup();
        }

		if (ImGui::BeginPopup("ErrMsg1"))
		{
			ImGui::Text("ОШИБКА: Была получена строка, а не число");
			
            ImGui::EndPopup();
        }

		if (ImGui::Button("Выбрать таблицу")) ImGui::OpenPopup("ListingObjects");

		if (selected_object != -1) {
			auto& table = ImGuiTalkBuffer::data.GetObjects()[selected_object];

			bool err1 = false;
			
			float chance = 0;
		    for (int i = 0; i < table.second.size(); ++i) {
				auto& unit = table.second[i];

				if (unit.err == Database::Error::None && (unit.type == Database::Type::Float || unit.type == Database::Type::Integer)) {
					chance += std::stof(unit.value);
				} else {
				    err1 = true;
				}
			}
			
			if (std::ceil(chance) != 1) {
				ImGui::OpenPopup("ErrMsg");

				selected_object = -1;
				
				return;
			}

			if (err1) {
				ImGui::OpenPopup("ErrMsg1");

				selected_object = -1;
				
				return;
			}

			std::vector<std::string> names;
			std::for_each(table.second.begin(), table.second.end(), [&](Database::Dataunit& u){
			    names.push_back(u.name);
			});

			static const char* current_name = "Ничего";
			
			static unsigned int val;
			MyGui::FastCombo("Вероятность", names, val, current_name);

			current_name = names[val].c_str();
			
			ImGui::Text((names[val] + " имеет вероятность " + table.second[val].value).c_str());

			static int interval_one;
			static int interval_two;

			ImGui::SliderInt("От", &interval_one, 1, names.size() - 1);
			ImGui::SliderInt("До", &interval_two, 1, names.size());

			if (interval_one > interval_two) interval_two = std::clamp(interval_two, interval_one + 1, int(names.size()));

			float sum = 0;
			std::for_each(table.second.begin() + interval_one, table.second.begin() + interval_two, [&](Database::Dataunit& u) {
				sum += std::stof(u.value);
			});
			
			ImGui::Text(("[" + std::to_string(interval_one) + ";" + std::to_string(interval_two) + "]" + " отрезок имеет вероятность " + std::to_string(sum)).c_str());
		}
	}
	
}
