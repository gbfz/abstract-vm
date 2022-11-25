#include "Token.hpp"
#include <utility>

token::token(eOperandType _type, std::unique_ptr<const IOperand> _value)
	: type(_type), value(std::exchange(_value, nullptr))
{}

token::token(token&& other)
	: type(other.type), value(std::exchange(other.value, nullptr))
{}

token& token::operator= (token&& other)
{
	type = other.type;
	value = std::exchange(other.value, nullptr);
	return *this;
}
