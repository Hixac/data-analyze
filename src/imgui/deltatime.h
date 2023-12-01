#pragma once

#include <chrono>

namespace MyGui {

    struct BasicInformation
	{
		inline static double deltatime = 0;
	};
	
	class SuicideCommand
	{
	public:
		SuicideCommand()
		{
			m_start = std::chrono::steady_clock::now();
		}
		
		~SuicideCommand()
		{
			m_end = std::chrono::steady_clock::now();
			BasicInformation::deltatime = std::chrono::duration<double>(m_end - m_start).count();
		}
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_start;
		std::chrono::time_point<std::chrono::steady_clock> m_end;
	};
	
}
