#include "InstructionStack.hpp"

void InstructionStack::push(eInstructionType instr)
{ emplace_back(instr); }

void InstructionStack::pop()
{ pop_back(); }

eInstructionType InstructionStack::top() const
{ return back(); }

InstructionStack::riter_t InstructionStack::begin()
{ return rbegin(); }

InstructionStack::riter_t InstructionStack::end()
{ return rend(); }

InstructionStack::criter_t InstructionStack::begin() const
{ return rbegin(); }

InstructionStack::criter_t InstructionStack::end() const
{ return rend(); }

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
