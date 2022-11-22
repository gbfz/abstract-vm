#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <vector>

struct ValueStack : protected std::vector<std::unique_ptr<const IOperand>>
{
	using base_t = std::vector<std::unique_ptr<const IOperand>>;
	using riter_t = base_t::reverse_iterator;
	using criter_t = base_t::const_reverse_iterator;

	void push(eOperandType type, const std::string& value);
	void pop();
	const IOperand& top() const;
	riter_t begin();
	riter_t end();
	const criter_t begin() const;
	const criter_t end() const;
	using base_t::clear;
};
