#pragma once
#include <cstdint>
#include <vector>
#include <ostream>

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

struct InstructionStack : protected std::vector<eInstructionType>
{
	using base_t = std::vector<eInstructionType>;
	using riter_t = base_t::reverse_iterator;
	using criter_t = base_t::const_reverse_iterator;

	void push(eInstructionType instr);
	void pop();
	eInstructionType top() const;
	riter_t begin();
	riter_t end();
	criter_t begin() const;
	criter_t end() const;
	using base_t::clear;
};
