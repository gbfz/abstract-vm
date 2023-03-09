#pragma once
#include "eOperandType.hpp"
#include <gmpxx.h>
#include <limits>

namespace avm {

template <std::integral T>
bool fits_impl(const auto& s)
{
	static auto const min = mpz_class(std::numeric_limits<T>::min());
	static auto const max = mpz_class(std::numeric_limits<T>::max());
	       auto const num = mpz_class(s);
	return num >= min && num <= max;
}

template <std::floating_point T>
bool fits_impl(const auto& s)
{
	static auto const min = mpf_class(std::numeric_limits<T>::lowest());
	static auto const max = mpf_class(std::numeric_limits<T>::max());
	       auto const num = mpf_class(s);
	return num >= min && num <= max;
}

}
