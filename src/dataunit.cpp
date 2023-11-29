#include <cstdio>
#include <dataunit.h>
#define DEBUG
#include <Base.h>

namespace Database {

	void Intersort::Add(const std::string& code, const std::vector<Dataunit> unit)
	{
		if (!FindCode(code))
		{
			m_Objects.push_back(std::pair(code, unit));
			return;
		}

		LOG_ERROR("Code already existing!");
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
			LOG_ERROR("No code!");
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

	Dataunit& Intersort::FindByName(const std::string& code, const std::string& name)
	{
		for (auto& object : m_Objects)
		{
			if (object.first == code)
				for (auto& unit : object.second)
				{
					if (unit.name == name)
						return unit;
				}
		}

		LOG_ERROR("No name either code!");
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

	void Intersort::Delete(const std::string& code, const std::string& name)
	{
	    for (auto& object : m_Objects)
		{
			if (object.first == code)			
				for (size_t i = 0; i < object.second.size(); i++)
				{
					if (object.second[i].name == name)
					{
						object.second.erase(object.second.begin() + i);
						return;
					}
				}		
		}

		LOG_ERROR("No code either name!");
	}
	
	void Intersort::Delete(const std::string& code)
	{
	    for (size_t i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects[i].first == code)
			{
				m_Objects.erase(m_Objects.begin() + i);
			    return;
			}
		}

		LOG_ERROR("No code!");
	}
}
