#pragma once
#include <boost/spirit/home/x3.hpp>
#include <vector>
#include <string>

namespace avm::parser {

namespace x3 = boost::spirit::x3;

struct push_back_each
{
	void operator() (auto& ctx)
	{
		auto vec = boost::get<std::vector<std::string>>(x3::_attr(ctx));
		for (auto& s : vec)
			x3::_val(ctx).emplace_back(std::move(s));
	}
};

struct push_back
{
	void operator() (auto& ctx)
	{
		auto&& str = boost::get<std::string>(x3::_attr(ctx));
		x3::_val(ctx).emplace_back(std::move(str));
	}
};

template <std::integral numT>
bool fits_impl(const std::string& s)
{
	auto min = std::to_string(std::numeric_limits<numT>::min());
	auto max = std::to_string(std::numeric_limits<numT>::max());
	return s >= min && s <= max;
}

template <std::floating_point numT>
bool fits_impl(const std::string& s)
{
	auto min = std::to_string(std::numeric_limits<numT>::lowest());
	auto max = std::to_string(std::numeric_limits<numT>::max());
	return s >= min && s <= max;
}

template <typename numT>
struct push_if_fits
{
	void operator() (auto& ctx)
	{
		auto&& s = boost::fusion::back(x3::_attr(ctx));
		if (fits_impl<numT>(s))
		{
			x3::_val(ctx).push_back(boost::fusion::front(x3::_attr(ctx)));
			x3::_val(ctx).emplace_back(std::move(s));
		}
		else x3::_pass(ctx) = false;
	}
};

template <class T>
auto make_rule(const char* name = "")
{
	return x3::rule<T, T> {name};
}

}
