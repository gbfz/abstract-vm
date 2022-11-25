#pragma once
#include <boost/spirit/home/x3.hpp>
#include "Token.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>
#undef assert // not my fault it's a macro

namespace avm::parser {

namespace x3 = boost::spirit::x3;

const auto N = x3::rule<class N, std::string> {"N"} = -x3::char_('-') >> +x3::digit;
const auto Z = x3::rule<class Z, std::string> {"Z"} = -x3::char_('-') >> +x3::digit >> '.' >> +x3::digit;

const auto value = x3::rule<class value, std::vector<std::string>> {"value"} =
				  (x3::string("int8")   >> '(' >> N >> ')')
				| (x3::string("int16")  >> '(' >> N >> ')')
				| (x3::string("int32")  >> '(' >> N >> ')')
				| (x3::string("float")  >> '(' >> Z >> ')')
				| (x3::string("double") >> '(' >> Z >> ')');

const auto comment = ';' >> -(x3::char_ - ';') >> *x3::char_;

static const auto push_back_each = [](auto& ctx)
{
	for (auto&& s : x3::_attr(ctx))
	{
		// std::cout << "|> " << s << '\n';
		x3::_val(ctx).emplace_back(std::move(s));
	}
};

static const auto push_back = [](auto& ctx)
{
	std::string out = x3::_attr(ctx);
	// std::cout << "%> " << out << '\n';
	x3::_val(ctx).emplace_back(std::move(out));
	// x3::_val(ctx).push_back(x3::_attr(ctx));
};

const auto push   = x3::rule<class push, std::vector<std::string>> {"push"}     = (x3::string("push")   >> +x3::lit(' ') >> value);
const auto assert = x3::rule<class assert, std::vector<std::string>> {"assert"} = (x3::string("assert") >> +x3::lit(' ') >> value);

const auto instr = x3::rule<class value, std::vector<std::string>> {"instr"} =
				 ( push                [push_back_each]
				 | assert              [push_back_each]
				 | x3::string("pop")   [push_back]
                 | x3::string("dump")  [push_back]
                 | x3::string("add")   [push_back]
                 | x3::string("sub")   [push_back]
                 | x3::string("mul")   [push_back]
                 | x3::string("div")   [push_back]
                 | x3::string("mod")   [push_back]
                 | x3::string("print") [push_back]
                 | x3::string("exit")  [push_back]
				 ) >> *comment;

const auto sep = +x3::char_('\n');

const auto S          = x3::rule<class S,         std::vector<std::string>> {"S"}          = instr % sep;
// const auto read_file  = x3::rule<class read_file, std::vector<std::string>> {"read_file"}  = S >> "exit";
// const auto read_stdin = x3::rule<class read_file, std::vector<std::string>> {"read_stdin"} = read_file >> ";;";
// BUT actually it all should produce a vector of tokens that i'd later consume without boost::spirit

template <class Iter>
bool parse_string(Iter& begin, Iter end, std::vector<std::string>& acc)
{
	auto parse_ok = x3::parse(begin, end, S, acc);
	if (!parse_ok)
		return false;
	return parse_ok;
}

}
