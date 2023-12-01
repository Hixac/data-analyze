#pragma once

namespace MyGui {

	class DynamicTimer
	{
	public:
		inline static DynamicTimer& Get() { static DynamicTimer timer; return timer; }
		
		void start(float time); // -> Nullifies time var and give permission to update
	    bool update(); // -> When time is full stops update
		
	private:
		bool m_Permission;
		float m_Time;
		
		DynamicTimer() = default;
		~DynamicTimer() = default;
		DynamicTimer(const DynamicTimer&&) = delete;
		DynamicTimer(const DynamicTimer&) = delete;
		void operator=(const DynamicTimer&) = delete;
	};
	
}
