#include "MachineStack.hpp"
#include "OperandFactory.hpp"
#include <list>
#include <stdexcept>
#include <utility>
#include "eOperandType.hpp"

using std::string_literals::operator""s;

namespace avm {

MachineStack::MachineStack()
{
	registers["A"] = nullptr;
	registers["B"] = nullptr;
}

MachineStack::MachineStack(MachineStack&& other)
{ *this = std::move(other); }

MachineStack& MachineStack::operator=(MachineStack&& other)
{
	if (this == &other)
		return *this;
	values = std::move(other.values);
	other.values.clear();
	registers = std::move(other.registers);
	other.registers["A"] = nullptr;
	other.registers["B"] = nullptr;
	return *this;
}

void MachineStack::push(std::unique_ptr<const IOperand> operand)
{ values.emplace_back(std::move(operand)); }

void MachineStack::push(std::list<std::string>& tokens)
{
	auto [type, value] = pop_two_tokens(tokens);
	push(OperandFactory::createOperand(type, std::move(value)));
}

void MachineStack::dump(std::ostream& out) const
{
	out << "dump #" << dump_n++ << '\n';
	// for (auto it = values.crbegin(); it != values.crend(); ++it)
		// out << "  -  " << (*it)->toString() << '\n';
}

void MachineStack::assert(eOperandType type, std::string value)
{
	if (values.empty())
		throw std::domain_error("Assert on empty stack");
	auto const& lhs = *values.back();
	if (lhs.getType() != type || lhs.toString() != value)
	{
		auto expected = toString(type) + "("s + value + ")"s;
		auto actual   = toString(lhs.getType()) + "("s + lhs.toString() + ")"s;
		throw std::domain_error("mismatch in assert(). expected "s + expected + ", got"s + actual);
	}
}

void MachineStack::assert(std::list<std::string>& tokens)
{
	auto [type, value] = pop_two_tokens(tokens);
	assert(type, std::move(value));
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
	if (lhs->toString().find_first_not_of(".0") == std::string::npos)
		throw std::invalid_argument("division by zero attempted");
	values.emplace_back(*rhs / *lhs);
}

void MachineStack::mod()
{
	auto [lhs, rhs] = pop_two();
	if (rhs->toString().find_first_not_of(".0") == std::string::npos)
		throw std::invalid_argument("modulo by zero attempted");
	values.emplace_back(*rhs % *lhs);
}

void MachineStack::print(std::ostream& out) const
{
	auto const& operand = *values.back();
	if (operand.getType() != eOperandType::Int8)
		throw std::invalid_argument("attempt to print value with type other than int8");
	unsigned char value = std::stoi(operand.toString());
	out << value << '\n';
}

void MachineStack::dup()
{
	if (values.size() == 0)
		throw std::out_of_range("attempt to dup value from empty stack");
	auto const& op = *values.back();
	push(OperandFactory::createOperand(op.getType(), op.toString()));
}

void MachineStack::save(std::list<std::string>& tokens)
{
	auto reg_name = std::move(tokens.front());
	tokens.pop_front();
	if (!registers.contains(reg_name))
		throw std::domain_error("saving to unknown register " + reg_name);
	registers[reg_name] = std::move(values.back());
	values.pop_back();
}

void MachineStack::load(std::list<std::string>& tokens)
{
	auto const& reg_name_check = tokens.front();
	if (!registers.contains(reg_name_check))
		throw std::domain_error("loading from unknown register");
	if (registers.at(reg_name_check) == nullptr)
		throw std::domain_error("loading from empty register");
	auto reg_name = std::move(tokens.front());
	tokens.pop_front();
	push(std::exchange(registers[reg_name], nullptr));
}

auto MachineStack::pop() -> std::unique_ptr<const IOperand>
{
	if (values.size() == 0)
		throw std::out_of_range("attempt to pop empty stack");
	auto operand = std::move(values.back());
	values.pop_back();
	return operand;
}

auto MachineStack::pop_two()
	-> std::pair<std::unique_ptr<IOperand const>,
				 std::unique_ptr<IOperand const>>
{
	if (values.size() < 2)
		throw std::out_of_range("attempt to pop 2 operands when stack size < 2");
	return { pop(), pop() };
}

auto MachineStack::pop_two_tokens(std::list<std::string>& tokens)
	-> std::pair<eOperandType, std::string>
{
	avm::eOperandType type { avm::toEnum(std::move(tokens.front())) };
	tokens.pop_front();
	std::string value { std::move(tokens.front()) };
	tokens.pop_front();
	return { type, value };
}

}

std::ostream& operator<< (std::ostream& out, avm::eOperandType type)
{
	using enum avm::eOperandType;
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
