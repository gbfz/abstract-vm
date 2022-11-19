#include "Adders.hpp"
#include <regex>
#include <tuple>

namespace rng = std::ranges;

namespace {

bool is_period(char c) { return c == '.'; }

auto makeFractionalParts(const std::string& lhs, const std::string& rhs) -> std::pair<std::string, std::string>
{
	const auto l = rng::find_if(lhs, is_period);
	const auto r = rng::find_if(rhs, is_period);
	std::string a { std::next(l), lhs.cend() };
	std::string b { std::next(r), rhs.cend() };
	if (a.size() == b.size())
		return { a, b };
	auto max_size = std::max(a.size(), b.size());
	rng::fill_n(std::back_inserter(a), max_size - a.size(), '0');
	rng::fill_n(std::back_inserter(b), max_size - b.size(), '0');
	return { a, b };
}

auto makeIntegralParts(const std::string& lhs, const std::string& rhs) -> std::pair<std::string, std::string>
{
	const auto l = rng::find_if(lhs, is_period);
	const auto r = rng::find_if(rhs, is_period);
	return {{ lhs.cbegin(), l }, { rhs.cbegin(), r }};
}

auto addFractional(const std::string& lhs, const std::string& rhs) -> std::pair<std::string, bool>
{
	auto r = addIntegral(lhs, rhs);
	auto rs = r.size();
	int no_carry = (rs == lhs.size() && rs == rhs.size())
				|| (rs == lhs.size() && r[0] == lhs[0])
				|| (rs == rhs.size() && r[0] == rhs[0]);
	if (no_carry)
		return { r, false };
	return { r.erase(0) + "0", true };
}

} // anonymous namespace 

std::string addFloating(std::string lhs, std::string rhs)
{
	std::tie(lhs, rhs) = detail::makeFloatStrings(lhs, rhs);
	const auto [fracA, fracB] = makeFractionalParts(lhs, rhs);
	const auto [intA, intB] = makeIntegralParts(lhs, rhs);
	const auto [fractional, carry] = addFractional(fracA, fracB);
	auto integral = addIntegral(intA, intB);
	if (carry)
		integral = addIntegral(integral, "1");
	return integral + "." + fractional;
}
