#pragma once
#include "eOperandType.hpp"
#include <gmpxx.h>
#include <limits>

namespace avm {

template <std::integral T>
bool fits_impl(const auto& s)
{
	static const auto min = mpz_class(std::numeric_limits<T>::min());
	static const auto max = mpz_class(std::numeric_limits<T>::max());
	const auto num = mpz_class(s);
	return num >= min && num <= max;
}

template <std::floating_point T>
bool fits_impl(const auto& s)
{
	static const auto min = mpf_class(std::numeric_limits<T>::lowest());
	static const auto max = mpf_class(std::numeric_limits<T>::max());
	const auto num = mpf_class(s);
	return num >= min && num <= max;
}

}
