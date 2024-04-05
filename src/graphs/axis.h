#include <vector>
#include <string>

namespace Graph {

	using RESULT = int;
	
	class Plot
	{
	public:
		Plot(int min, int max, float precision);

		void Update(const std::vector<std::string>& exprs, std::vector<std::pair<std::vector<double>, std::vector<double>>>* populus = nullptr);
		
	private:
	    RESULT Relate(const std::string& expr, std::vector<double>& x, std::vector<double>& y) const;

		int m_min, m_max;
		float m_precision;
	};
	
}
