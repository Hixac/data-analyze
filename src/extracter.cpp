#include <parser.h>
#include <string>
#define DEBUG
#include <Base.h>

namespace File {

	Extracter::Extracter(const std::string& filepath)
	{
		m_File.open(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
		
		ReadLines();
	}

	Extracter::~Extracter()
	{
		m_File.close();
	}
	
	void Extracter::ReadLines()
	{
		if (m_File)
		{
			std::string line;
			while (std::getline(m_File, line))
				m_Lines.push_back(line);
			m_File.clear();
		}
		else { LOG_ERROR("File isn't opened!"); }
	}

	std::string Extracter::WholeRead()
	{
		if (m_File)
		{
			std::string text;
			std::string line;

			GotoLine(0, STATE::Input);
			while (std::getline(m_File, line))
			    text += line + '\n';
			m_File.clear();
			return text;
		}
		else { LOG_ERROR("File isn't opened!"); }
	}
	
	std::string Extracter::ReadLine(size_t line)
	{
		GotoLine(line, STATE::Input);
		std::string strLine;
		std::getline(m_File, strLine);

		m_File.clear();
		return strLine;
	}
	
	void Extracter::GotoLine(unsigned int num, STATE state)
	{
		switch (state)
		{
		case STATE::Input:
			m_File.seekg(num, std::ios::beg);
			break;
		case STATE::Output:
			m_File.seekp(num, std::ios::beg);
			break;
		}
	}
}
