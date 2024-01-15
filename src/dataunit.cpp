#include <cstdio>
#include <dataunit.h>

namespace Database {

	void Intersort::Add(const std::string& code, const std::vector<Dataunit> unit)
	{
		if (!FindCode(code))
		{
			m_Objects.push_back(std::pair(code, unit));
			return;
		}
		else
		{
			for (auto& data : m_Objects)
			{
				if (data.first == code)
				{
					data.second = unit;
				}
			}
		}
	}

	void Intersort::AddIntoExisting(const std::string& code, Dataunit unit)
	{
		std::vector<Dataunit>* p_data = nullptr;
		
		for (auto& data : m_Objects)
		{
			if (data.first == code)
			{
			    p_data = &data.second;
			}
		}
		if (p_data == nullptr)
		{
		    return;
		}

		for (auto& dunit : *p_data)
		{
			if (dunit.name == unit.name)
			{
				dunit.value = unit.value;
				return;
			}
		}

		p_data->push_back(unit);
	}

	bool Intersort::FindByName(const std::string& code, const std::string& name, Dataunit& dunit)
	{
		for (auto& object : m_Objects)
		{
			if (object.first == code)
				for (auto& unit : object.second)
				{
					if (unit.name == name)
					{
						dunit = unit;
						return true;
					}
				}
		}

		return false;
	}

	bool Intersort::FindCode(const std::string& code) const
	{
		for (auto& object : m_Objects)
		{
			if (object.first == code)
				return true;
		}
		return false;
	}

	bool Intersort::Delete(const std::string& code, const std::string& name)
	{		
	    for (auto& object : m_Objects)
		{
			if (object.first == code)			
				for (size_t i = 0; i < object.second.size(); i++)
				{
					if (object.second[i].name == name)
					{
						object.second.erase(object.second.begin() + i);
						return true;
					}
				}		
		}

		return false;
	}
	
    bool Intersort::Delete(const std::string& code)
	{
	    for (size_t i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects[i].first == code)
			{
				m_Objects.erase(m_Objects.begin() + i);
			    return true;
			}
		}

	    return false;
	}
}
