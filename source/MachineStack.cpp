#include "MachineStack.hpp"
#include "IOperand.hpp"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

MachineStack::MachineStack(MachineStack&& other)
	: vs(std::move(other.vs))
{
	other.vs.clear();
}

MachineStack& MachineStack::operator=(MachineStack&& other)
{
	vs = std::move(other.vs);
	other.vs.clear();
	return *this;
}

void MachineStack::push(eOperandType type, std::string value)
{
	auto operand = OperandFactory::createOperand(type, std::move(value));
	vs.emplace_back(std::move(operand));
}

void MachineStack::push(std::unique_ptr<const IOperand> operand)
{ vs.emplace_back(std::move(operand)); }

void MachineStack::dump() const
{
	for (auto&& operand : vs)
		std::cout << operand->toString() << '\n';
}

void MachineStack::assert(eOperandType type, std::string value)
{
	const auto& lhs = *vs.back();
	std::stringstream s;
	if (lhs.getType() != type)
	{
		s << "Type mismatch in assert. ";
		s << "Expected '" << type << "', got '" << lhs.getType() << "'";
		throw std::runtime_error(s.str());
	}
	if (lhs.toString() != value)
	{
		s << "Value mismatch in assert. ";
		s << "Expected '" << value << "', got '" << lhs.toString() << "'";
		throw std::runtime_error(s.str());
	}
	/*
	if (lhs.getType() != type)
		throw std::runtime_error("Type mismatch in assert");
	if (lhs.toString() != value)
		throw std::runtime_error("Value mismatch in assert");
	*/
}

/*
void MachineStack::assert()
{
	auto [lhs, rhs] = pop_two();
	auto l_type = lhs->getType();
	auto r_type = rhs->getType();
	const auto& l_val = lhs->toString();
	const auto& r_val = rhs->toString();
	vs.emplace_back(std::move(rhs));
	vs.emplace_back(std::move(lhs));
	if (l_type != r_type)
		throw std::runtime_error("Type mismatch in assert");
	if (l_val != r_val)
		throw std::runtime_error("Value mismatch in assert");
}
*/

void MachineStack::add()
{
	auto [lhs, rhs] = pop_two();
	try {
		vs.emplace_back(*lhs + *rhs);
	}
	catch (std::exception& e) {
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw;
	}
}

void MachineStack::sub()
{
	auto [lhs, rhs] = pop_two();
	try {
		vs.emplace_back(*lhs - *rhs);
	}
	catch (std::exception& e) {
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw;
	}
}

void MachineStack::mul()
{
	auto [lhs, rhs] = pop_two();
	try {
		vs.emplace_back(*lhs * *rhs);
	}
	catch (std::exception& e) {
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw;
	}
}

void MachineStack::div()
{
	auto [lhs, rhs] = pop_two();
	if (lhs->toString().find_first_not_of(".0") == std::string::npos)
	{
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw std::runtime_error("Division by zero");
	}
	try {
		vs.emplace_back(*rhs / *lhs);
	}
	catch (std::exception& e)
	{
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw;
	}
}

void MachineStack::mod()
{
	auto [lhs, rhs] = pop_two();
	if (lhs->toString().find_first_not_of(".0") == std::string::npos)
	{
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw std::runtime_error("Division by zero");
	}
	try {
		vs.emplace_back(*rhs % *lhs);
	}
	catch (std::exception& e)
	{
		vs.emplace_back(std::move(rhs));
		vs.emplace_back(std::move(lhs));
		throw;
	}
}

void MachineStack::print() const
{
	const auto& operand = *vs.back();
	if (operand.getType() != eOperandType::Int8)
		throw std::runtime_error("Printing value whose type != Int8");
	int32_t v = std::stoi(operand.toString());
	unsigned char value = std::stoi(operand.toString());
	std::cout << v << ", " << value << '\n';
}

void MachineStack::pop()
{ vs.pop_back(); }

std::pair<MachineStack::ptr_t, MachineStack::ptr_t>
MachineStack::pop_two()
{
	if (vs.size() < 2)
		throw std::runtime_error("Not enough operands on stack");
	auto a = std::move(vs.back()); vs.pop_back();
	auto b = std::move(vs.back()); vs.pop_back();
	return { std::move(a), std::move(b) };
}

/*
void MachineStack::exec()
{
	using enum eInstructionType;
	while (!is.empty())
	{
		switch (is.back())
		{
			case Push: break;
			case Pop: pop(); break;
			case Dump: dump(); break;
			case Assert: assert(); break;
			case Add: add(); break;
			case Sub: sub(); break;
			case Mul: mul(); break;
			case Div: div(); break;
			case Mod: mod(); break;
			case Print: print(); break;
			case Exit: exit(0xAB0BA);
		}
		is.pop_back();
	}
}

void MachineStack::fill(std::vector<eInstructionType>&& instructions,
						std::vector<ptr_t>&& operands)
{
	using enum eInstructionType;
	while (!instructions.empty())
	{
		is.emplace_back(instructions.back());
		instructions.pop_back();
		if (is.back() == Push || is.back() == Assert)
		{
			vs.emplace_back(std::move(operands.back()));
			operands.pop_back();
		}
	}
}
*/

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
