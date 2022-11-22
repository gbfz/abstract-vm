#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
// #include "InstructionStack.hpp"
// #include "ValueStack.hpp"
#include <utility>
#include <iostream>
#include <unordered_map>
#include <vector>

enum class eInstructionType : int8_t
{
	Push,
	Pop,
	Dump,
	Assert,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Print,
	Exit,
};

std::ostream& operator<< (std::ostream& other, eInstructionType instr);

class MachineStack
{
public:
	using ptr_t = std::unique_ptr<const IOperand>;
// stack instructions 
	void push(eInstructionType i_type,
			  eOperandType o_type, std::string value);
	void push(eInstructionType type);
	void push(std::unique_ptr<const IOperand> operand);
	void pop();
	void dump() const;
	void assert();
	void add();
	void sub();
	void mul();
	void div();
	void mod();
	void print() const;

	void exec();
	void fill(std::vector<eInstructionType>&& instructions,
			  std::vector<ptr_t>&& operands);

// ctors, dtors
	MachineStack() = default;
	// MachineStack(InstructionStack&& is, ValueStack&& vs) = delete;
	MachineStack(const MachineStack&) = delete;
	MachineStack& operator= (const MachineStack&) = delete;
	MachineStack(MachineStack&& other);
	MachineStack& operator= (MachineStack&& other);
	~MachineStack() = default;

// private:
	std::pair<ptr_t, ptr_t> pop_two();
// member fields 
	std::vector<eInstructionType> is;
	std::vector<ptr_t> vs;
	// InstructionStack is;
	// ValueStack vs;
};

std::ostream& operator<< (std::ostream& out, eOperandType type);
