#include "Adders.hpp"
#include <tuple>
#include <regex>

namespace {

auto eraseDecimalPoints(std::string& lhs, std::string& rhs)
{
	lhs.erase(lhs.find('.'), 1);
	rhs.erase(rhs.find('.'), 1);
}

auto getFractionalPartSize(const std::string& lhs, const std::string& rhs)
{
	const auto a = lhs.find('.') + 1;
	const auto b = rhs.find('.') + 1;
	return std::make_pair(lhs.size() - a, rhs.size() - b);
}

auto trimZeros(std::string& s)
{
	static auto trailing_zeros = std::regex("\\d+\\.[0-9]+0$");
	while (std::regex_search(s, trailing_zeros))
		s.erase(s.size() - 1, 1);
	static const auto leading_zeros = std::regex("^0\\d");
	while (std::regex_search(s, leading_zeros))
		s.erase(0, 1);
	return s;
}

}

std::string mulFloating(std::string lhs, std::string rhs)
{
	std::tie(lhs, rhs) = detail::makeFloatStrings(lhs, rhs);
	const auto [fracSizeA, fracSizeB] = getFractionalPartSize(lhs, rhs);
	eraseDecimalPoints(lhs, rhs);
	auto out = mulIntegral(lhs, rhs);
	const auto decimalPointOffset = out.size() - (fracSizeA + fracSizeB);
	out.insert(decimalPointOffset, 1, '.');
	return trimZeros(out);
}
