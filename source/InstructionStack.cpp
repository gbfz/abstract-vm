#include "InstructionStack.hpp"

void InstructionStack::push(eInstructionType instr)
{ emplace_back(instr); }

void InstructionStack::pop()
{ pop_back(); }

eInstructionType InstructionStack::top() const
{ return back(); }

auto InstructionStack::cbegin() const
{ return crbegin(); }

auto InstructionStack::cend() const
{ return crend(); }
