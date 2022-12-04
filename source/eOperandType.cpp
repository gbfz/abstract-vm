#include "eOperandType.hpp"
#include <unordered_map>

namespace avm {

using enum eOperandType;

std::string toString(eOperandType type)
{
	const std::unordered_map<eOperandType, std::string> tm = {
		{ eOperandType::Int8,   "int8"   },
		{ eOperandType::Int16,  "int16"  },
		{ eOperandType::Int32,  "int32"  },
		{ eOperandType::Float,  "float"  },
		{ eOperandType::Double, "double" },
	};
	return tm.at(type);
}

eOperandType toEnum(const std::string& type)
{
	static const std::unordered_map<std::string, eOperandType> tm = {
		{ "int8",   eOperandType::Int8   },
		{ "int16",  eOperandType::Int16  },
		{ "int32",  eOperandType::Int32  },
		{ "float",  eOperandType::Float  },
		{ "double", eOperandType::Double },
	};
	return tm.at(type);
}

}
