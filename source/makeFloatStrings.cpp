#include "Adders.hpp"
#include <regex>

namespace detail {

auto makeFloatStrings(const std::string& lhs, const std::string& rhs)
	-> std::pair<std::string, std::string>
{
	auto to_float = [](const std::string& s)
	{
		static const auto flt = std::regex("\\d+\\.\\d+");
		if (std::regex_search(s, flt))
			return s;
		if (s.starts_with('.'))
			return "0" + s;
		if (s.ends_with('.'))
			return s + "0";
		return s + ".0";
	};
	return std::make_pair(to_float(lhs), to_float(rhs));
}

}
