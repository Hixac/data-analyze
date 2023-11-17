#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace File {

    enum class STATE { Input, Output };
	
	class Extracter
	{
	public:
		Extracter(const std::string& filepath);
		~Extracter();

		std::string ReadLine(size_t line);
		std::string WholeRead();
		
		inline std::vector<std::string>& GetLines() { return m_Lines; }
		
	private:
	    void ReadLines();
		void GotoLine(unsigned int num, STATE state);
		
		std::vector<std::string> m_Lines;
		
		std::fstream m_File;
	};
	
}
