#pragma once
#include "InstructionStack.hpp"
#include "ValueStack.hpp"
#include <utility>
#include <iostream>

class MachineStack
{
public:
// stack instructions 
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

// ctors, dtors
	MachineStack() = default;
	MachineStack(const MachineStack&) = delete;
	MachineStack& operator= (const MachineStack&) = delete;
	MachineStack(MachineStack&& other);
	MachineStack& operator= (MachineStack&& other);
	~MachineStack() = default;

private:
	InstructionStack is;
	ValueStack vs;
};

std::ostream& operator<< (std::ostream& out, eOperandType type);
