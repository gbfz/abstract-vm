#pragma once
#include "eOperandType.hpp"
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <array>

namespace ft {

namespace {

	template <class T>
	using lim = std::numeric_limits<T>;

	using int8  = lim<int8_t>;
	using int16 = lim<int16_t>;
	using int32 = lim<int32_t>;
	using flt   = lim<float>;
	using dbl   = lim<double>;

	auto str(auto i)
	{ return std::to_string(i); }

}

template <class T>
bool didOverflow(const T& r, eOperandType type) requires (!std::is_same_v<T, std::string>)
{
	using enum eOperandType;
	switch (type)
	{
		case Int8:   return r < int8::min()   || r > int8::max();
		case Int16:  return r < int16::min()  || r > int16::max();
		case Int32:  return r < int32::min()  || r > int32::max();
		case Float:  return r < flt::lowest() || r > flt::max();
		case Double: return r < dbl::lowest() || r > dbl::max();
	}
	throw std::logic_error("Unsupported type in overflow check");
}

template <class T>
bool didOverflow(const T& r, eOperandType type) requires (std::is_same_v<T, std::string>)
{
	using enum eOperandType;
	static const std::unordered_map<eOperandType, std::pair<std::string, std::string>> tm = {
		{ Int8,   { str(int8::min()),   str(int8::max())  } },
		{ Int16,  { str(int16::min()),  str(int16::max()) } },
		{ Int32,  { str(int32::min()),  str(int32::max()) } },
		{ Float,  { str(flt::lowest()), str(flt::max())   } }, 
		{ Double, { str(dbl::lowest()), str(dbl::max())   } },
	};
	const auto& [min, max] = tm.at(type);
	return r < min || r > max;
}

}
