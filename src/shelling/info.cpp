#include <shelling/info.h>

#include <imgui/deltatime.h>

namespace Shell {

	void Info::OnUpdate()
	{
		if (m_showWindow)
			UpdateWrap();
	}
	
	void Info::UpdateWrap()
	{
		ImGui::SetNextWindowBgAlpha(1);
		ImGui::Begin(m_label.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::SameLine();
		ImGui::Text("Программа создана для расчёта и визуализации математических функций на\n основе базы данных с возможностью манипулировать ими, указывать типы вводимых\n данных и создавать новые таблицы с данными и функциями. Может проводить\n анализ статистических данных на графиках, исследовать зависимости, выявлять\n тренды, аномалии или другие интересующие факторы.");
		
		ImGui::End();
	}
	
}
