#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <type_traits>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <concepts>

class MachineStack
{
	using ptr_t = std::unique_ptr<const IOperand>;
public:
// stack instructions 
	void push(eOperandType type, std::string value);
	void push(ptr_t operand);
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

// ctors, dtors
	MachineStack() = default;
	MachineStack(const MachineStack&) = delete;
	MachineStack& operator= (const MachineStack&) = delete;
	MachineStack(MachineStack&& other);
	MachineStack& operator= (MachineStack&& other);
	~MachineStack() = default;

private:
	std::pair<ptr_t, ptr_t> pop_two();
	std::vector<ptr_t> values;
};

std::ostream& operator<< (std::ostream& out, eOperandType type);
