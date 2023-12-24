#pragma once

#include <memory>

#include <extracter.h>
#include <dataunit.h>
#include <lexer.h>

namespace File {

	class Parser
	{
	public:
		Parser(Extracter& file);
		
	    void ReadData(Database::Intersort& data);
	    void WriteData(Database::Intersort& data);
		
	private:
		std::unique_ptr<Extracter> m_File;
	};
	
}
