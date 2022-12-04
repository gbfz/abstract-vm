#pragma once
#include <cstdint>
#include <string>

namespace avm {

enum class eOperandType : int8_t
{
	Int8,
	Int16,
	Int32,
	Float,
	Double,
};

std::string toString(eOperandType type);
eOperandType toEnum(const std::string& t);

}
