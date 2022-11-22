#include "ValueStack.hpp"

void ValueStack::push(eOperandType type, const std::string& value)
{
	auto&& operand = OperandFactory::createOperand(type, value);
	emplace_back(std::move(operand));
}

void ValueStack::pop()
{ pop_back(); }

const IOperand& ValueStack::top() const
{ return *back(); }

auto ValueStack::cbegin() const
{ return crbegin(); }

auto ValueStack::cend() const
{ return crend(); }
