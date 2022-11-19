#pragma once
#include <string>
#include <algorithm>
#include <ranges>
#include <initializer_list>
#include <vector>

namespace detail {

	auto makeFloatStrings(const std::string& lhs, const std::string& rhs)
		-> std::pair<std::string, std::string>;

}

std::string addIntegral(const std::string& lhs, const std::string& rhs);
std::string addFloating(std::string lhs, std::string rhs);

std::string mulIntegral(const std::string& lhs, const std::string& rhs);
std::string mulFloating(std::string lhs, std::string rhs);

std::string subIntegral(const std::string& lhs, const std::string& rhs);
std::string subFloating(const std::string& lhs, const std::string& rhs);

std::string divIntegral(const std::string& lhs, const std::string& rhs);
std::string divFloating(const std::string& lhs, const std::string& rhs);

std::string modIntegral(const std::string& lhs, const std::string& rhs);
std::string modFloating(const std::string& lhs, const std::string& rhs);
