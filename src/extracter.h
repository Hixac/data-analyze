#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace File {
	
	class Extracter
	{
	public:
		Extracter(const std::string& filepath);
		~Extracter();
		
		std::string ReadLine(size_t line);

	    void SetContent(const std::string& content);
		inline std::string GetContent() const { return m_Content; }
		
	private:
		std::string WholeRead();
		void GotoLine(size_t num);
		
		std::string m_Content;

		std::string m_Filepath;
		std::ifstream m_File;
	};
	
}
