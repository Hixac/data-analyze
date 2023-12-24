#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include <parser.h>

namespace File {

	void WhiteSpaceRemoval(std::string& s)
	{	
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] == ' ')
			{
				s.erase(s.begin() + i);
			}
		}
	}
	
	Parser::Parser(Extracter& file)
	{
		m_File = std::unique_ptr<Extracter>(&file);
	}

	void Parser::ReadData(Database::Intersort& data)
	{
		std::string source = m_File->GetContent();
		Lexer lexer(source);

		std::vector<Symbol> symbs = lexer.GetAllSymbols();
		
		std::vector<Database::Object> objects;
		Database::Object object;

		for (int i = 0; i < symbs.size(); i++)
		{
			WhiteSpaceRemoval(symbs[i].value);
		}
		
	    for (int i = 0; i < symbs.size(); i++)
		{
			if (symbs[i].token == HighTokens::ObjectName)
			{
				if (!object.first.empty()) objects.push_back(object);
				object.first = symbs[i].value;
			}
			if (symbs[i].token == HighTokens::AttributeName)
			{
				if (symbs[i + 1].token != HighTokens::AttributeType)
					object.second.push_back({symbs[i].value, symbs[i + 1].value});
				else
				{
					Database::Type type;
					std::string typeStr = symbs[i + 1].value;

					if (typeStr == "int")
						type = Database::Type::Integer;
					else if (typeStr == "float")
						type = Database::Type::Float;
					else type = Database::Type::String;
					
					object.second.push_back({symbs[i].value, symbs[i + 2].value, type});
				}
			}
	    }
		data.GetObjects() = objects;
	}
	
	void Parser::WriteData(Database::Intersort& data)
	{
		
	}
}
