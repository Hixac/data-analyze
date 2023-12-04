#include <imgui/dynamictimer.h>
#include <imgui.h>

namespace MyGui {

	DynamicTimer::DynamicTimer(float time)
	{
		m_Time = ImGui::GetTime() + time;
		m_Permission = true;
	}
	
	void DynamicTimer::start(float time)
	{
		m_Time = ImGui::GetTime() + time;
		m_Permission = true;
	}

	bool DynamicTimer::update()
	{
		if (m_Permission && m_Time < ImGui::GetTime())
		{
			m_Permission = false;
			return true;
		}
		return false;
	}
}
