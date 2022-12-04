#pragma once
#include "IOperand.hpp"

namespace avm {

template <eOperandType Type>
class Operand;

class OperandFactory
{
private:
	static std::unique_ptr<const IOperand>
	createInt8(const std::string& value);

	static std::unique_ptr<const IOperand>
	createInt16(const std::string& value);

	static std::unique_ptr<const IOperand>
	createInt32(const std::string& value);

	static std::unique_ptr<const IOperand>
	createFloat(const std::string& value);

	static std::unique_ptr<const IOperand>
	createDouble(const std::string& value);

public:
	static std::unique_ptr<const IOperand>
	createOperand(eOperandType type, const std::string& value);
};

}
