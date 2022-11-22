#pragma once
#include <string>
#include <memory>

enum class eOperandType
{
	Int8,
	Int16,
	Int32,
	Float,
	Double,
};

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
