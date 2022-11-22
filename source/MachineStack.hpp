#pragma once
#include "InstructionStack.hpp"
#include "ValueStack.hpp"

class MachineStack
{
	void push(eInstructionType i_type,
			  eOperandType o_type, std::string value);
	void push(eInstructionType type);
	void pop();
	void dump() const;
	void assert(eOperandType type, std::string value) const;
	void add();
	void sub();
	void mul();
	void div();
	void mod();
	void print() const;
private:
	InstructionStack is;
	ValueStack vs;
};
