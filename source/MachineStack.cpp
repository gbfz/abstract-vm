#include "MachineStack.hpp"
#include "Exceptions.hpp"

MachineStack::MachineStack(MachineStack&& other) : values()
{ *this = std::move(other); }

MachineStack& MachineStack::operator=(MachineStack&& other)
{
	if (this == &other)
		return *this;
	values.clear();
	values = std::move(other.values);
	other.values.clear();
	return *this;
}

void MachineStack::push(eOperandType type, std::string value)
{
	auto operand = OperandFactory::createOperand(type, std::move(value));
	values.emplace_back(std::move(operand));
}

void MachineStack::push(std::unique_ptr<const IOperand> operand)
{ values.emplace_back(std::move(operand)); }

void MachineStack::dump(std::ostream& out) const
{
	for (auto&& operand : values)
		out << operand->getType() << "(" << operand->toString() << ")\n";
}

void MachineStack::assert(eOperandType type, std::string value)
{
	const auto& lhs = *values.back();
	std::stringstream s;
	if (lhs.getType() != type)
		throw ft::type_assert_exception(type, lhs.getType());
	if (lhs.toString() != value)
		throw ft::value_assert_exception(value, lhs.toString());
}

void MachineStack::assert(std::unique_ptr<const IOperand> op)
{
	assert(op->getType(), op->toString());
};

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
		throw ft::div_by_zero_exception();
	values.emplace_back(*rhs / *lhs);
}

void MachineStack::mod()
{
	auto [lhs, rhs] = pop_two();
	if (rhs->toString().find_first_not_of(".0") == std::string::npos)
		throw ft::mod_by_zero_exception();
	values.emplace_back(*rhs % *lhs);
}

void MachineStack::print(std::ostream& out) const
{
	const auto& operand = *values.back();
	if (operand.getType() != eOperandType::Int8)
		throw ft::print_exception();
	unsigned char value = std::stoi(operand.toString());
	out << value << '\n';
}

std::unique_ptr<const IOperand> MachineStack::pop()
{
	if (values.size() == 0)
		throw ft::pop_exception();
	auto operand = std::move(values.back());
	values.pop_back();
	return operand;
}

std::pair<std::unique_ptr<const IOperand>,
		  std::unique_ptr<const IOperand>>
MachineStack::pop_two()
{
	if (values.size() < 2)
		throw ft::pop_two_exception();
	return { pop(), pop() };
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
