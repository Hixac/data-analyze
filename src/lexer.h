#pragma once

namespace File {

	enum class Tokenizer
	{
		None, Undefined, Space,
		Saving, Commiting, Commentary,
		StartObject, EndObject
	};
	
	class Lexer
	{
	public:
		static Tokenizer SymbolResolver(const char symbol)
		{
			switch (symbol)
			{
			case '[':
				return Tokenizer::StartObject;
			case ']':
				return Tokenizer::EndObject;
			case '|':
				return Tokenizer::Commentary;
			case '\n':
				return Tokenizer::Commiting;
			case '=':
				return Tokenizer::Saving;
			case ' ':
				return Tokenizer::Space;
			default:
				return Tokenizer::Undefined;
			}
		}

		inline static Tokenizer LastToken = Tokenizer::None;
	private:
		Lexer() = default;
	    ~Lexer() = default;
	};
	
}
