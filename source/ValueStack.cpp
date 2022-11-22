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

ValueStack::riter_t ValueStack::begin()
{ return rbegin(); }

ValueStack::riter_t ValueStack::end()
{ return rend(); }

const ValueStack::criter_t ValueStack::begin() const
{ return rbegin(); }

const ValueStack::criter_t ValueStack::end() const
{ return rend(); }
