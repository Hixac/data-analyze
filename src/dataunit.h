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
		void Add(std::string code, std::vector<Dataunit> units);
		void AddIntoExisting(std::string code, Dataunit unit);
		void Delete(std::string code);

		inline std::vector<std::pair<std::string, std::vector<Dataunit>>>& GetObjects() { return m_Objects; }
		
	private:
		std::vector<std::pair<std::string, std::vector<Dataunit>>> m_Objects;
	};
	
}
