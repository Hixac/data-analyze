#include <vector>
#include <memory>
#include <string>

#include <parser.h>

namespace File {

	Parser::Parser(std::unique_ptr<Extracter>& file)
	{
		m_pFile = &file;
	}

	void Parser::ReadData(Database::Intersort& data)
	{
		std::string source = (*m_pFile)->GetContent();
		Lexer lexer(source);

		std::vector<std::vector<Symbol>> symbs = lexer.GetAllSymbols();
		
		std::vector<Database::Object> objects;
		Database::Object object;
		
	    for (auto& item : symbs)
		{
			object.first = item[0].value;
			Database::Dataunit unit;
			for (int i = 1; i < item.size(); i++)
			{
			    Symbol sym = item[i];
				if (sym.token == HighTokens::AttributeName)
				{
					unit.name = sym.value;
					continue;
				}
				else if (sym.token == HighTokens::AttributeType)
				{
				    if (sym.value == "int")
						unit.type = Database::Type::Integer;
					if (sym.value == "float")
						unit.type = Database::Type::Float;
					if (sym.value == "str")
						unit.type = Database::Type::String;
					if (sym.value == "point")
						unit.type = Database::Type::Point;
					continue;
				}
				else
				{
					unit.value = sym.value;
					object.second.push_back(unit);
					continue;
				}				
			}
			objects.push_back(object);
			object.second.clear();
		}
		
		data.SetObjects(objects);
	}
	
	void Parser::WriteData(Database::Intersort& data)
	{
		std::string content = "";
		for (auto& object : data.GetObjects())
		{
			content += "[" + object.first + "]\n";
			for (auto& item : object.second)
			{
				std::string type;
				if (item.type == Database::Type::Integer)
					type = "int";
				else if (item.type == Database::Type::Float)
					type = "float";
				else if (item.type == Database::Type::Point)
					type = "point";
				else type = "str";
				
				if (item.IsInt(item.value)) {
					type = "int";
				} else if (item.IsFloat(item.value)) {
					type = "float";
				}
				
				std::string err = "";
				item.ErrChk();
				if (item.err == Database::Error::WrongType)
					err = " | ERROR! WRONG TYPE!";
				
				content += item.name + ": " + type + " = " + item.value + err + "\n";
			}
		}
		content += "[END]\n";
		
		(*m_pFile)->SetContent(content);
	}
}
