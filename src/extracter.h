#pragma once

#include <fstream>
#include <string>

namespace File {
	
	class Extracter
	{
	public:
#if defined(_WIN32)
		Extracter(const std::wstring& filepath);
#else
		Extracter(const std::string& filepath);
#endif
		~Extracter();
		
		std::string ReadLine(size_t line);

	    void SetContent(const std::string& content);
		inline const std::string& GetContent() const { return m_Content; }
		
	private:
		std::string WholeRead();
		void GotoLine(size_t num);

		bool IsEmpty();
		
		std::string m_Content;

#if defined(_WIN32)
		std::wstring m_Filepath;
#else
		std::string m_FilePath;
#endif
		std::ifstream m_File;
	};
	
}
