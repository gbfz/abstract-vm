#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <stdexcept>
#include <sstream>
#include <vector>
#include <iostream>

class MachineStack
{
using ptr_t = std::unique_ptr<const IOperand>;
public:
// stack instructions 
	void push(eOperandType type, std::string value);
	ptr_t pop();
	void dump(std::ostream& out = std::cout) const;
	void assert(eOperandType type, std::string value);
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
	void push(ptr_t operand);
	std::pair<ptr_t, ptr_t> pop_two();

// fields 
	std::vector<ptr_t> values;
};

std::ostream& operator<< (std::ostream& out, eOperandType type);
