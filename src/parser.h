#pragma once

#include <extracter.h>
#include <memory>
#include <dataunit.h>

namespace File {

	class Parser
	{
	public:
		Parser(Extracter& file);
		Database::Intersort Process();
		
	private:
		std::unique_ptr<Extracter> m_File;
	};
	
}
