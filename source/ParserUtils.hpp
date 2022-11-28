#pragma once
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <gmpxx.h>

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
	static const auto min = mpz_class(std::numeric_limits<numT>::min());
	static const auto max = mpz_class(std::numeric_limits<numT>::max());
	auto num = mpz_class(s);
	return num >= min && num <= max;
}

template <std::floating_point numT>
bool fits_impl(const std::string& s)
{
	static const auto min = mpf_class(std::numeric_limits<numT>::min());
	static const auto max = mpf_class(std::numeric_limits<numT>::max());
	auto num = mpf_class(s);
	return num >= min && num <= max;
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
