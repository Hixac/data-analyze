#pragma once

#include <string>
#include <vector>

namespace Database {

	enum class Type { Integer, Float, String };
	
    struct Dataunit
	{
		std::string name;
		std::string value;
		Type type = Type::String;
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

		inline void SetObjects(std::vector<Object> objects) { m_Objects = objects; }
		inline std::vector<Object>& GetObjects() { return m_Objects; }
		
	private:
		std::vector<Object> m_Objects;
	};
	
}
