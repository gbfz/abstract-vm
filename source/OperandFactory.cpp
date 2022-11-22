#include "OperandFactory.hpp"
#include "Operand.hpp"
#include <unordered_map>

using enum eOperandType;

std::unique_ptr<const IOperand>
OperandFactory::createInt8(const std::string& value)
{
	return std::make_unique<Operand<Int8>>(value);
}

std::unique_ptr<const IOperand>
OperandFactory::createInt16(const std::string& value)
{
	return std::make_unique<Operand<Int16>>(value);
}

std::unique_ptr<const IOperand>
OperandFactory::createInt32(const std::string& value)
{
	return std::make_unique<Operand<Int32>>(value);
}

std::unique_ptr<const IOperand>
OperandFactory::createFloat(const std::string& value)
{
	return std::make_unique<Operand<Float>>(value);
}

std::unique_ptr<const IOperand>
OperandFactory::createDouble(const std::string& value)
{
	return std::make_unique<Operand<Double>>(value);
}

std::unique_ptr<const IOperand>
OperandFactory::createOperand(eOperandType type, const std::string& value)
{
	using func_t = decltype(&OperandFactory::createInt8);
	static const std::unordered_map<eOperandType, func_t> func = {
		{ Int8,   createInt8   },
		{ Int16,  createInt16  },
		{ Int32,  createInt32  },
		{ Float,  createFloat  },
		{ Double, createDouble },
	};
	return func.at(type)(value);
}
