#pragma once

#include <fstream>
#include <string>

namespace File {
	
	class Extracter
	{
	public:
		Extracter(const std::string& filepath);
		~Extracter();

		void OpenFile(const std::string& filepath);
		
		std::string ReadLine(size_t line);

	    void SetContent(const std::string& content);
		inline const std::string& GetContent() const { return m_Content; }
		
	private:
		std::string WholeRead();
		void GotoLine(size_t num);

		bool IsEmpty();
		
		std::string m_Content;

		std::string m_Filepath;
		std::ifstream m_File;
	};
	
}
