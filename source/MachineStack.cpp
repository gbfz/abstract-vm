#include "MachineStack.hpp"
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
	is.push(i_type);
	vs.push(o_type, std::move(value));
}

void MachineStack::push(eInstructionType type)
{ is.push(type); }

void MachineStack::pop()
{ vs.pop(); }

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
