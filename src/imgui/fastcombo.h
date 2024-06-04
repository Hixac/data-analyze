#pragma once

#include <string>
#include <vector>

#include <imgui.h>

namespace MyGui {
	
	inline void FastCombo(const char* label, std::vector<std::string>& values, unsigned int& value, const char* preview_val)
	{
		if (ImGui::BeginCombo(label, preview_val))
		{
			for (size_t i = 0; i < values.size(); i++)
			{
				const bool is_selected = (value == i);
				if (ImGui::Selectable(values[i].c_str(), is_selected))
					value = i;
				
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}
