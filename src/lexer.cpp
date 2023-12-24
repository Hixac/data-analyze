#include <lexer.h>

#include <array>

namespace File {

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
		for (int i = 0; i < m_Tokens.size() - 1; i++)
		{
			if (m_Tokens[i].first == Tokenizer::StartObject)
				m_Symbs.push_back({m_Text.substr(m_Tokens[i].second + 1, m_Tokens[i + 1].second - m_Tokens[i].second - 1), HighTokens::ObjectName});
			if (m_Tokens[i].first == Tokenizer::NewLine && (m_Symbs[m_Symbs.size() - 1].token == HighTokens::ObjectName || m_Symbs[m_Symbs.size() - 1].token == HighTokens::AttributeVal))
				m_Symbs.push_back({m_Text.substr(m_Tokens[i].second + 1, m_Tokens[i + 1].second - m_Tokens[i].second - 1), HighTokens::AttributeName});
			if (m_Tokens[i].first == Tokenizer::Type)
				m_Symbs.push_back({m_Text.substr(m_Tokens[i].second + 1, m_Tokens[i + 1].second - m_Tokens[i].second - 1), HighTokens::AttributeType});
			if (m_Tokens[i].first == Tokenizer::Assign)
				m_Symbs.push_back({m_Text.substr(m_Tokens[i].second + 1, m_Tokens[i + 1].second - m_Tokens[i].second - 1), HighTokens::AttributeVal});
		}
	}
}
