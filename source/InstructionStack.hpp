#pragma once
#include <cstdint>
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

struct InstructionStack : protected std::vector<eInstructionType>
{
	void push(eInstructionType instr);
	void pop();
	eInstructionType top() const;
	auto cbegin() const;
	auto cend() const;
};
