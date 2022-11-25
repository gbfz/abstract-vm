#pragma once
#include <string>
#include <memory>
#include "eOperandType.hpp"

class IOperand
{
public:
	virtual int getPrecision() const = 0;
	virtual eOperandType getType() const = 0;

	virtual std::unique_ptr<const IOperand>
	operator+ (const IOperand& rhs) const = 0;

	virtual std::unique_ptr<const IOperand>
	operator- (const IOperand& rhs) const = 0;

	virtual std::unique_ptr<const IOperand>
	operator* (const IOperand& rhs) const = 0;

	virtual std::unique_ptr<const IOperand>
	operator/ (const IOperand& rhs) const = 0;

	virtual std::unique_ptr<const IOperand>
	operator% (const IOperand& rhs) const = 0;

	virtual const std::string& toString() const = 0;

	virtual ~IOperand() {}
};
