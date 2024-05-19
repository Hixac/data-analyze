#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include <Base.h>

namespace Database {

	enum class Type { Integer, Float, String, Point };
	enum class Error { None, WrongType };
	
    struct Dataunit
	{
		std::string name;
		std::string value;
		Type type = Type::String;
		Error err = Error::None;

		void ErrChk()
		{
			if (type == Type::Integer)
			{
				if (!IsInt(value))
					err = Error::WrongType;
			}
			else if (type == Type::Float)
			{
				if (!IsFloat(value))
					err = Error::WrongType;
			}
			else if (type == Type::Point)
			{
			    std::string::size_type pos = value.find(';');
				if (pos != std::string::npos && value.size() > pos + 1)
				{
					if (!IsFloat(value.substr(0, pos))) err = Error::WrongType;
					if (!IsFloat(value.substr(pos + 1, value.size()))) err = Error::WrongType;
				}
				else
					err = Error::WrongType;
			}
		}

		bool GetPointers(std::pair<float, float>& pair)
		{
			if (type == Type::Point && err != Error::WrongType && value.size() != 0)
			{
				std::string::size_type pos = value.find(';');
				pair = { std::stof(value.substr(0, pos)), std::stof(value.substr(pos + 1, value.size())) };
				return true;
			}

			return false;
		}

	    bool IsInt(const std::string& s)
		{
		    if (s.size() == 0 || (s.size() == 1 && s[0] == '-')) return false;
			std::string temp = s;
			if (s[0] == '-') temp.erase(temp.begin());
			return std::all_of(temp.begin(), temp.end(), [&](char c) {
				return ::isdigit(static_cast<unsigned char>(c));
			});
		}

		bool IsFloat(const std::string& s)
		{
			if (s.size() == 0 || (s.size() == 1 && s[0] == '-')) return false;
			bool dot = true;
			std::string temp = s;
			if (s[0] == '-') temp.erase(temp.begin());
			return std::all_of(temp.begin(), temp.end(), [&](char c) {
				return (isdigit(static_cast<unsigned char>(c)) || (c == '.' && dot && (dot = false) == false)); });
		}
	};

	using Object = std::pair<std::string, std::vector<Dataunit>>;

	// Въебал столько констант, шо ебанёшься
	class Intersort
	{
	public:
		void Add(const std::string& code, const std::vector<Dataunit> units);
		void AddIntoExisting(const std::string& code, Dataunit unit);
		
	    bool Delete(const std::string& code);
	    bool Delete(const std::string& code, const std::string& name);
		bool FindByName(const std::string& code, const std::string& name, Dataunit& dunit);
		bool FindCode(const std::string& code) const;

		inline void SetObjects(std::vector<Object> objects) { m_Objects = objects; }
		inline std::vector<Object>& GetObjects() { return m_Objects; }
		
	private:
		std::vector<Object> m_Objects;
	};
	
}
