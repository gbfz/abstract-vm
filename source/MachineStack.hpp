#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <utility>
#include <iostream>
#include <unordered_map>
#include <vector>

class MachineStack
{
public:
	using ptr_t = std::unique_ptr<const IOperand>;
// stack instructions 
	void push(eOperandType type, std::string value);
	void push(std::unique_ptr<const IOperand> operand);
	void pop();
	void dump() const;
	void assert(eOperandType type, std::string value);
	void assert();
	void add();
	void sub();
	void mul();
	void div();
	void mod();
	void print() const;
	void exec();

// ctors, dtors
	MachineStack() = default;
	MachineStack(const MachineStack&) = delete;
	MachineStack& operator= (const MachineStack&) = delete;
	MachineStack(MachineStack&& other);
	MachineStack& operator= (MachineStack&& other);
	~MachineStack() = default;

// private:
	std::pair<ptr_t, ptr_t> pop_two();
// member fields 
	std::vector<ptr_t> vs;
	// ValueStack vs;
};

std::ostream& operator<< (std::ostream& out, eOperandType type);
