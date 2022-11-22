#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <vector>

struct ValueStack : protected std::vector<std::unique_ptr<const IOperand>>
{
	void push(eOperandType type, const std::string& value);
	void pop();
	const IOperand& top() const;
	auto cbegin() const;
	auto cend() const;
};
