#pragma once

#include <climits>
#include <vector>
#include <array>
#include <string>

#include <Base.h>

class Yard
{
	struct Presedence { unsigned int index; std::string op; float operand1; float operand2; };

	using ERROR_RESULT = int;
	
public:
	Yard() = default;
	Yard(std::string expression, int x = INT_MAX);
	
    ERROR_RESULT result(float& answer);
	float GetVal(Presedence p);

    Presedence ClearFunction(unsigned int index);
    Presedence FindHighestPresedence();

	void WhiteSpaceRemoval(std::string& s);

	bool IsNumber(const std::string& s);	
	bool IsWord(const std::string& s);
	bool IsOper(const std::string& s);
	bool IsFunction(const std::string& s);
	
	void SetTokens(std::vector<std::string> tokens) { this->tokens = tokens; }
	
private:
	std::array<std::string, 18> opers {"abs", "cos", "sin", "tg", "arccos", "arcsin", "arctg", "ch", "sh", "th", "arch", "arsh", "arth", "^", "/", "*", "-", "+"};
	std::vector<std::string> tokens;
};
