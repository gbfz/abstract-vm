#include "MachineStack.hpp"
#include "IOperand.hpp"
#include <exception>
#include <string>
#include <sstream>

MachineStack& MachineStack::operator=(MachineStack&& other)
{
	if (this == &other)
		return *this;
	values.clear();
	values = std::move(other.values);
	other.values.clear();
	return *this;
}

MachineStack::MachineStack(MachineStack&& other) : values()
{ *this = std::move(other); }

void MachineStack::push(eOperandType type, std::string value)
{
	auto operand = OperandFactory::createOperand(type, std::move(value));
	values.emplace_back(std::move(operand));
}

void MachineStack::push(std::unique_ptr<const IOperand> operand)
{ values.emplace_back(std::move(operand)); }

void MachineStack::dump() const
{
	for (auto&& operand : values)
		std::cout << operand->getType() << "(" << operand->toString() << ")\n";
}

void MachineStack::assert(eOperandType type, std::string value)
{
	const auto& lhs = *values.back();
	std::stringstream s;
	if (lhs.getType() != type)
	{
		s << "Type mismatch in assert. "
		  << "Expected '" << type
		  << "', got '" << lhs.getType() << "'";
		throw std::runtime_error(s.str());
	}
	if (lhs.toString() != value)
	{
		s << "Value mismatch in assert. "
		  << "Expected '" << value
		  << "', got '" << lhs.toString() << "'";
		throw std::runtime_error(s.str());
	}
}

void MachineStack::add()
{
	auto [lhs, rhs] = pop_two();
	values.emplace_back(*lhs + *rhs);
}

void MachineStack::sub()
{
	auto [lhs, rhs] = pop_two();
	values.emplace_back(*lhs - *rhs);
}

void MachineStack::mul()
{
	auto [lhs, rhs] = pop_two();
	values.emplace_back(*lhs * *rhs);
}

void MachineStack::div()
{
	auto [lhs, rhs] = pop_two();
	if (rhs->toString().find_first_not_of(".0") == std::string::npos)
		throw std::runtime_error("Division by zero");
	values.emplace_back(*rhs / *lhs);
}

void MachineStack::mod()
{
	auto [lhs, rhs] = pop_two();
	if (rhs->toString().find_first_not_of(".0") == std::string::npos)
		throw std::runtime_error("Taking modulo of zero");
	values.emplace_back(*rhs % *lhs);
}

void MachineStack::print() const
{
	const auto& operand = *values.back();
	if (operand.getType() != eOperandType::Int8)
		throw std::runtime_error("Printing value whose type isn't Int8");
	unsigned char value = std::stoi(operand.toString());
	std::cout << value << '\n';
}

void MachineStack::pop()
{
	if (values.size() == 0)
		throw std::runtime_error("Attempt to pop empty stack");
	values.pop_back();
}

std::pair<std::unique_ptr<const IOperand>, std::unique_ptr<const IOperand>>
MachineStack::pop_two()
{
	if (values.size() < 2)
		throw std::runtime_error("Not enough operands on stack");
	auto rhs = std::move(values.back()); values.pop_back();
	auto lhs = std::move(values.back()); values.pop_back();
	return { std::move(rhs), std::move(lhs) };
}

std::ostream& operator<< (std::ostream& out, eOperandType type)
{
	using enum eOperandType;
	switch (type)
	{
		case Int8:   return out << "int8";
		case Int16:  return out << "int16";
		case Int32:  return out << "int32";
		case Float:  return out << "float";
		case Double: return out << "double";
	}
	return out;
}
