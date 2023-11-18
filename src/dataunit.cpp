#include <dataunit.h>

namespace Database {

	void Intersort::Add(std::string code, std::vector<Dataunit> unit)
	{
	    m_Objects.push_back(std::pair(code, unit));
	}

	void Intersort::Delete(std::string code)
	{
	    for (size_t i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects[i].first == code)
			{
				m_Objects.erase(m_Objects.begin() + i);
				break;
			}
		}
	}
}
