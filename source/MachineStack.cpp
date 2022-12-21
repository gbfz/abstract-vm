#include "MachineStack.hpp"
#include "OperandFactory.hpp"
#include <list>
#include <utility>
#include "eOperandType.hpp"

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
	for (auto it = values.crbegin(); it != values.crend(); ++it)
		out << "  -  " << (*it)->toString() << '\n';
}

void MachineStack::assert(eOperandType type, std::string value)
{
	if (values.empty())
		throw std::runtime_error("Assert on empty stack");
	const auto& lhs = *values.back();
	if (lhs.getType() != type)
		throw avm::type_assert_exception(type, lhs.getType());
	if (lhs.toString() != value)
		throw avm::value_assert_exception(value, lhs.toString());
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
		throw avm::div_by_zero_exception();
	values.emplace_back(*rhs / *lhs);
}

void MachineStack::mod()
{
	auto [lhs, rhs] = pop_two();
	if (rhs->toString().find_first_not_of(".0") == std::string::npos)
		throw avm::mod_by_zero_exception();
	values.emplace_back(*rhs % *lhs);
}

void MachineStack::print(std::ostream& out) const
{
	const auto& operand = *values.back();
	if (operand.getType() != eOperandType::Int8)
		throw avm::print_exception();
	unsigned char value = std::stoi(operand.toString());
	out << value << '\n';
}

void MachineStack::dup()
{
	if (values.size() == 0)
		throw avm::pop_exception(); // TODO: replace with dup analog
	const auto& op = *values.back();
	push(OperandFactory::createOperand(op.getType(), op.toString()));
}

void MachineStack::save(const std::string& reg_name)
{
	if (!registers.contains(reg_name))
		throw std::runtime_error("saving to unknown register " + reg_name);
	registers[reg_name] = std::move(values.back());
	values.pop_back();
}

void MachineStack::save(std::list<std::string>& tokens)
{
	auto reg_name = std::move(tokens.front());
	tokens.pop_front();
	if (!registers.contains(reg_name))
		throw std::runtime_error("saving to unknown register " + reg_name);
	registers[reg_name] = std::move(values.back());
	values.pop_back();
}

void MachineStack::load(const std::string& reg_name)
{
	if (!registers.contains(reg_name))
		throw std::runtime_error("loading from unknown register");
	if (registers.at(reg_name) == nullptr)
		throw std::runtime_error("loading from empty register");
	push(std::exchange(registers[reg_name], nullptr));
}

void MachineStack::load(std::list<std::string>& tokens)
{
	auto reg_name = std::move(tokens.front());
	tokens.pop_front();
	if (!registers.contains(reg_name))
		throw std::runtime_error("loading from unknown register");
	if (registers.at(reg_name) == nullptr)
		throw std::runtime_error("loading from empty register");
	push(std::exchange(registers[reg_name], nullptr));
}

std::unique_ptr<const IOperand> MachineStack::pop()
{
	if (values.size() == 0)
		throw avm::pop_exception();
	auto operand = std::move(values.back());
	values.pop_back();
	return operand;
}

std::pair<std::unique_ptr<const IOperand>,
		  std::unique_ptr<const IOperand>>
MachineStack::pop_two()
{
	if (values.size() < 2)
		throw avm::pop_two_exception();
	return { pop(), pop() };
}

std::pair<eOperandType, std::string>
MachineStack::pop_two_tokens(std::list<std::string>& tokens)
{
	auto type = avm::toEnum(std::move(tokens.front()));
	tokens.pop_front();
	auto value = std::move(tokens.front());
	tokens.pop_front();
	return { std::move(type), std::move(value) };
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
