#include "OverflowCheck.hpp"

namespace ft {

bool didOverflow(const std::string& r, eOperandType type)
{
	using enum eOperandType;
	using std::to_string;
	static const std::unordered_map<eOperandType, std::pair<std::string, std::string>> tm = {
		{ Int8,   { to_string(int8::min()),   to_string(int8::max())  } },
		{ Int16,  { to_string(int16::min()),  to_string(int16::max()) } },
		{ Int32,  { to_string(int32::min()),  to_string(int32::max()) } },
		{ Float,  { to_string(flt::lowest()), to_string(flt::max())   } }, 
		{ Double, { to_string(dbl::lowest()), to_string(dbl::max())   } },
	};
	const auto& [min, max] = tm.at(type);
	return r < min || r > max;
}

}
