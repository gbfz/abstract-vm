#include "Adders.hpp"

namespace rng = std::ranges;

namespace {

char mul(char a, char b, int& carry)
{
	int c = (a - '0') * (b - '0') + carry;
	carry = c / 10;
	if (c > 9)
		return c % 10 + '0';
	return c + '0';
}

std::string makeFactor(auto&& from, char by, int fill = 0)
{
	int carry = 0;
	std::string f = "";
	for (char digit : from | std::views::reverse)
		f += mul(digit, by, carry);
	if (carry)
		f += std::to_string(carry);
	rng::reverse(f);
	rng::fill_n(std::back_inserter(f), fill++, '0');
	return f;
}

} // anonymous namespace

std::string mulIntegral(const std::string& lhs, const std::string& rhs)
{
	auto collect_factors = [fill = 0](auto&& rng1, auto&& rng2) mutable
	{
		std::vector<std::string> fs;
		for (char i : rng1 | std::views::reverse)
		{
			auto&& factor = makeFactor(rng2, i, fill++);
			fs.emplace_back(std::move(factor));
		}
		return fs;
	};
	auto sum_factors = [](auto&& factors)
	{
		std::string sum = "";
		for (auto&& f : factors)
			sum = addIntegral(sum, f);
		return sum;
	};
	std::string out = sum_factors(collect_factors(lhs, rhs));
	if (out.find_first_not_of("0") == std::string::npos)
		return "0";
	return out;
}
