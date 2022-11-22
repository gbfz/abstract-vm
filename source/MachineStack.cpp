#include "MachineStack.hpp"
#include "IOperand.hpp"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>

MachineStack::MachineStack(MachineStack&& other)
	: is(std::move(other.is))
	, vs(std::move(other.vs))
{
	other.is.clear();
	other.vs.clear();
}

MachineStack& MachineStack::operator=(MachineStack&& other)
{
	is = std::move(other.is);
	other.is.clear();
	vs = std::move(other.vs);
	other.vs.clear();
	return *this;
}

void MachineStack::push(eInstructionType i_type,
						eOperandType o_type,
						std::string value)
{
	is.emplace_back(i_type);
	auto operand = OperandFactory::createOperand(o_type, value);
	vs.emplace_back(std::move(operand));
}

void MachineStack::push(eInstructionType type)
{ is.emplace_back(type); }

void MachineStack::push(std::unique_ptr<const IOperand> operand)
{ vs.emplace_back(std::move(operand)); }

void MachineStack::pop()
{
	is.pop_back();
	vs.pop_back();
}

void MachineStack::dump() const
{
	using enum eInstructionType;
	for (auto val_iter = vs.begin(); auto instr : is)
	{
		std::cout << instr;
		if (instr == Push || instr == Assert)
		{
			const auto type = (*val_iter)->getType();
			const auto& val = (*val_iter)->toString();
			std::cout << ' ' << type << "(" << val << ")";
			++val_iter;
		}
		std::cout << '\n';
	}
}

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

std::pair<MachineStack::ptr_t, MachineStack::ptr_t>
MachineStack::pop_two()
{
	if (vs.size() < 2)
		throw std::runtime_error("Not enough operands on stack");
	auto a = std::move(vs.back()); vs.pop_back();
	auto b = std::move(vs.back()); vs.pop_back();
	return { std::move(a), std::move(b) };
}

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

std::ostream& operator<< (std::ostream& out, eInstructionType instr)
{
	using enum eInstructionType;
	switch (instr)
	{
		case Push:   return out << "push";
		case Pop:    return out << "pop";
		case Dump:   return out << "dump";
		case Assert: return out << "assert";
		case Add:    return out << "add";
		case Sub:    return out << "sub";
		case Mul:    return out << "mul";
		case Div:    return out << "div";
		case Mod:    return out << "mod";
		case Print:  return out << "print";
		case Exit:   return out << "exit";
	}
	return out;
}
