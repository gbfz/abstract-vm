#pragma once
#include <boost/spirit/home/x3.hpp>
#include "OverflowCheck.hpp"
#include <vector>
#include <list>
#include <string>

namespace avm::parser {

namespace x3 = boost::spirit::x3;
namespace fn = boost::fusion;

template <typename numT>
struct push_if_fits
{
	void operator() (auto& ctx)
	{
		auto&& s = fn::back(x3::_attr(ctx));
		if (fits_impl<numT>(s))
		{
			x3::_val(ctx).push_back(fn::front(x3::_attr(ctx)));
			x3::_val(ctx).emplace_back(std::move(s));
		}
		else x3::_pass(ctx) = false;
	}
};

struct push_back_each
{
	void operator() (auto& ctx)
	{
		auto in = boost::get<std::list<std::string>>(x3::_attr(ctx));
		for (auto&& s : in)
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

}
