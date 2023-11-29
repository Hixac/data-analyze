#pragma once

#include <string>
#include <vector>

namespace Database {

    struct Dataunit
	{
		std::string name;
		std::string value;
	};

	using Object = std::pair<std::string, std::vector<Dataunit>>;
	
	class Intersort
	{
	public:
		void Add(const std::string& code, const std::vector<Dataunit> units);
		void AddIntoExisting(const std::string& code, Dataunit unit);
		void Delete(const std::string& code);
		void Delete(const std::string& code, const std::string& name);

		Dataunit& FindByName(const std::string& code, const std::string& name);
		bool FindCode(const std::string& code) const;
		
		inline std::vector<Object>& GetObjects() { return m_Objects; }
		
	private:
		std::vector<Object> m_Objects;
	};
	
}
