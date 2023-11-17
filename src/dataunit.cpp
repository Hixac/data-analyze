#include <dataunit.h>

namespace Database {

	void Intersort::Add(std::string code, std::vector<Dataunit> unit)
	{
		m_Objects[code] = unit;
	}

	void Intersort::Delete(std::string code)
	{
		m_Objects.erase(code);
	}
}
