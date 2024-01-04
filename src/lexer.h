#pragma once

#include <Base.h>

#include <vector>
#include <string>

namespace File {

	enum class Tokenizer
	{
	    Undefined,
	    NewLine, Assign, StartObject, EndObject, Commentary, Type
	};

	enum class HighTokens
	{
		ObjectName, AttributeName, AttributeVal, AttributeType
	};
	
	struct Symbol
	{
		std::string value;
	    HighTokens token;
	};
	
	class Lexer
	{
	public:
		Lexer(std::string& text);
	    ~Lexer() = default;
		
		inline std::vector<std::vector<Symbol>>& GetAllSymbols() { return m_Symbs; }
		
	private:
		std::vector<std::vector<Symbol>> m_Symbs;
		std::vector<std::pair<Tokenizer, size_t>> m_Tokens;
		
		std::string& m_Text;
		size_t m_CursorPos;
		
	    void ResolveTokens();
	    void ResolveNextSymbol();
		void AnalyzeSymbols();
	};
	
}
