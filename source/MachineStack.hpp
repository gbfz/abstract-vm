#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include "Exceptions.hpp"
#include <vector>
#include <iostream>

class MachineStack
{
using ptr_t = std::unique_ptr<const IOperand>;
public:
// stack instructions 
	void push(eOperandType type, std::string value);
	void push(ptr_t operand);
	ptr_t pop();
	void dump(std::ostream& out = std::cout) const;
	void assert(eOperandType type, std::string value);
	void assert(ptr_t operand);
	void add();
	void sub();
	void mul();
	void div();
	void mod();
	void print(std::ostream& out = std::cout) const;

// ctors, dtors
	MachineStack() = default;
	MachineStack(const MachineStack&) = delete;
	MachineStack& operator= (const MachineStack&) = delete;
	MachineStack(MachineStack&& other);
	MachineStack& operator= (MachineStack&& other);
	~MachineStack() = default;

private:
// utils 
	std::pair<ptr_t, ptr_t> pop_two();

// fields 
	std::vector<ptr_t> values;
	mutable size_t dump_n = 0;
};

std::ostream& operator<< (std::ostream& out, eOperandType type);
