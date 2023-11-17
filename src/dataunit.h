#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Database {

    struct Dataunit
	{
		std::string name;
		std::string value;
	};

	class Intersort
	{
	public:
		void Add(std::string code, std::vector<Dataunit> unit);
		void Delete(std::string code);

		inline std::unordered_map<std::string, std::vector<Dataunit>>& GetObjects() { return m_Objects; }
		
	private:
		std::unordered_map<std::string, std::vector<Dataunit>> m_Objects;
	};
	
}
