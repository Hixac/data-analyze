#include <dataunit.h>
#include <stdexcept>

namespace Database {

	void Intersort::Add(std::string code, std::vector<Dataunit> unit)
	{
	    m_Objects.push_back(std::pair(code, unit));
	}

	void Intersort::AddIntoExisting(std::string code, Dataunit unit)
	{
		bool is_found = false;
		for (auto& data : m_Objects)
		{
			if (data.first == code)
			{
				data.second.push_back(unit);
				is_found = true;
			}
		}

		if (!is_found)
			throw std::invalid_argument("Didn't found code!");
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
