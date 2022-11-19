#include "Adders.hpp"

namespace rng = std::ranges;

std::string addIntegral(const std::string& lhs, const std::string& rhs)
{
	bool carry = false;
	auto add_n = [&carry](auto... args)
	{
		int c = '0' + carry;
		for (char d : { args... })
			c += d - '0';
		if (carry = c > '9'; carry)
			return c - 0xA;
		return c;
	};
	std::string out;
	out.reserve(std::max(lhs.size(), rhs.size()));
	auto inserter = std::back_inserter(out);
	auto r = rng::transform(lhs | std::views::reverse,
							rhs | std::views::reverse,
							inserter,
							add_n);
	rng::transform(r.in1, lhs.crend(), inserter, add_n);
	rng::transform(r.in2, rhs.crend(), inserter, add_n);
	if (carry)
		out += "1";
	rng::reverse(out);
	if (out.find_first_not_of("0") == std::string::npos)
		return "0";
	return out;
}
