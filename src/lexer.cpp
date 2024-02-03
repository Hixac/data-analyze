#include <lexer.h>

#include <array>

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
	
	Lexer::Lexer(std::string& text)
		: m_Text(text), m_CursorPos(0)
	{
		ResolveTokens();
	}

    void Lexer::ResolveTokens()
	{
		while (m_CursorPos < m_Text.size())
		    ResolveNextSymbol();
		m_CursorPos = 0;
		AnalyzeSymbols();
	}
	
    void Lexer::ResolveNextSymbol()
	{
	    char symbol = m_Text[m_CursorPos];
	    
		if (symbol == '\n')
			m_Tokens.push_back({Tokenizer::NewLine, m_CursorPos});
		if (symbol == '=')
			m_Tokens.push_back({Tokenizer::Assign, m_CursorPos});
		if (symbol == ':')
			m_Tokens.push_back({Tokenizer::Type, m_CursorPos});
		if (symbol == '[')
		    m_Tokens.push_back({Tokenizer::StartObject, m_CursorPos});
		if (symbol == ']')
			m_Tokens.push_back({Tokenizer::EndObject, m_CursorPos});
		if (symbol == '|')
			m_Tokens.push_back({Tokenizer::Commentary, m_CursorPos});

		m_CursorPos++;
	}
	
	void Lexer::AnalyzeSymbols()
	{
		std::vector<Symbol> symbols;
		for (int i = 0; i < m_Tokens.size() - 1; i++)
		{
			std::string slice = m_Text.substr(m_Tokens[i].second + 1, m_Tokens[i + 1].second - m_Tokens[i].second - 1);
			WhiteSpaceRemoval(slice);
			if (m_Tokens[i].first == Tokenizer::StartObject)
			{
				if (!symbols.empty())
				{
					m_Symbs.push_back(symbols);
					symbols.clear();
				}
			    symbols.push_back({slice, HighTokens::ObjectName});
			}
			if (m_Tokens[i].first == Tokenizer::NewLine && i + 1 < m_Tokens.size() && m_Tokens[i + 1].first != Tokenizer::StartObject)
			    symbols.push_back({slice, HighTokens::AttributeName});
			if (m_Tokens[i].first == Tokenizer::Type)
			    symbols.push_back({slice, HighTokens::AttributeType});
			if (m_Tokens[i].first == Tokenizer::Assign)
				symbols.push_back({slice, HighTokens::AttributeVal});
		}
	}
}
