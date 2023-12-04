#pragma once

namespace MyGui {

	class DynamicTimer
	{
	public:
		DynamicTimer(float time);
		DynamicTimer() = default;
		~DynamicTimer() = default;
		
		void start(float time); // -> Nullifies time var and give permission to update
	    bool update(); // -> When time is full stops update
		
	private:
		bool m_Permission = false;
		float m_Time;
	};
	
}
