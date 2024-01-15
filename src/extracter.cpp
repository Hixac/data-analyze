#include <fstream>
#include <ostream>
#include <parser.h>
#include <string>

#include <Base.h>

namespace File {

	Extracter::Extracter(const std::string& filepath)
		: m_Filepath(filepath)
	{
		m_File.open(filepath, std::fstream::in);

		if (!m_File.is_open())
		{
			std::ofstream out(filepath, std::ios::out | std::ios::trunc);
			
			out << "[Таблица1]\n";
			out << "x1: int = 1\n";
			out << "x2: float = 1.025\n";
			out << "x3: point = 1.23;200\n";
			out << "[Таблица2]\n";
			out << "name: str = Тостер\n";
			out << "name: str = Григорий\n";
			out << "name: str = Дима\n";
			out << "surname: str = Горелов\n";
			out << "[END]\n";
			out.close();

			m_File.open(filepath, std::ios::in | std::ios::app);
		}
		
		m_Content = WholeRead();
	}

	Extracter::~Extracter()
	{
		m_File.close();
	}

	void Extracter::SetContent(const std::string& content)
	{
		m_Content = content;
		
		std::ofstream output(m_Filepath, std::ios::out | std::ios::trunc);
		
		if (!output.is_open())
		{
			LOG_ERROR("Something went wrong rewriting file!");
		}
		
	    output << m_Content;
		output.close();
		m_File.clear();
	}

	std::string Extracter::WholeRead()
	{
		if (m_File)
		{
			std::string text;
			std::string line;

			GotoLine(0);
			while (std::getline(m_File, line))
			    text += line + '\n';
			m_File.clear();
			return text;
		}
		LOG_ERROR("File isn't opened!");
	}
	
	std::string Extracter::ReadLine(size_t line)
	{
		GotoLine(line);
		std::string strLine;
		std::getline(m_File, strLine);

		m_File.clear();
		return strLine;
	}
	
	void Extracter::GotoLine(size_t num)
	{
		m_File.seekg(num, std::ios::beg);
	}
}
