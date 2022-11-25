#pragma once
#include "eOperandType.hpp"
#include "IOperand.hpp"

struct token
{
	eOperandType type;
	std::unique_ptr<const IOperand> value;

	token() = delete;
	token(const token&) = delete;
	token& operator= (const token&) = delete;

	token(eOperandType type, std::unique_ptr<const IOperand> value);
	token(token&& other);
	token& operator= (token&& other);
};
