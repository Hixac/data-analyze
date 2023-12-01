#include <exception>
#include <lexer.h>

#include <memory>
#include <parser.h>
#include <stdexcept>
#include <string>

namespace File {

	Parser::Parser(Extracter& file)
	{
		m_File = std::unique_ptr<Extracter>(&file);
	}

	Database::Intersort Parser::Process()
	{
		Database::Intersort data;
		
		std::string tempName = "";
		Database::Dataunit unit;
		std::vector<Database::Dataunit> units;
		bool startObjectFlag = false;
		bool startSaving = false;
		
		const std::string fileText = m_File->GetContent();
		for (size_t i = 0; i < fileText.length(); i++)
		{
			switch(Lexer::SymbolResolver(fileText[i]))
			{
			case Tokenizer::StartObject:
				startObjectFlag = true;
				if (!tempName.empty()) data.Add(tempName, units);
				units.clear();
				tempName = "";
				break;
			case Tokenizer::EndObject:
				if (tempName == "END") return data; /* <-- return */
				startObjectFlag = false;
				break;
			case Tokenizer::Undefined:
				if (startObjectFlag)   tempName += fileText[i];
				else if (!startSaving) unit.name += fileText[i];
				else if (startSaving)  unit.value += fileText[i];
				break;
			case Tokenizer::Saving:
				startSaving = true;
				break;
			case Tokenizer::Commiting:
				if (startSaving)
				{
					units.push_back(unit);
				    unit.name = ""; unit.value = "";
				}
				startSaving = false;
				break;
			case Tokenizer::Commentary:
				continue;
			case Tokenizer::Space:
				break;
			case Tokenizer::None:
				break;
			}
		}
	    throw std::invalid_argument("No \"[END]\" in database");
	}
	
	void Parser::BackProcess(Database::Intersort& intersort)
	{
		std::string result = "";
		
		for (auto& data : intersort.GetObjects())
		{
			result += "[" + data.first + "]\n";
			for (auto& unit : data.second)
				result += unit.name + " = " + unit.value + "\n";
		}
		result += "[END]";
		m_File->SetContent(result);
	}
}