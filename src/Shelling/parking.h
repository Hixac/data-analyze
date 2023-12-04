#include <Shelling/mainshell.h>

#include <window.h>

namespace Shell {

	class Parking : public Shell
	{
	public:
		Parking(Window& window);

		void OnUpdate() override;

		void CreateTableInput(std::string& buffer, size_t id);
		void Label(const char* name, bool left = false);
		
		template<typename __first, typename __last>
		std::vector<__first> GetFirstOfPairs(std::vector<std::pair<__first, __last>> data);

	private:
	    Window* m_Window;
	};
			
}
