#pragma once
#include <boost/spirit/home/x3.hpp>
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
	auto vec = boost::get<std::vector<std::string>>(x3::_attr(ctx));
	for (auto& s : vec)
		x3::_val(ctx).emplace_back(std::move(s));
};

static const auto push_back = [](auto& ctx)
{
	auto&& str = boost::get<std::string>(x3::_attr(ctx));
	x3::_val(ctx).emplace_back(std::move(str));
};

const auto space = x3::lit(' ') | x3::lit('\t');

const auto push   = x3::rule<class push,   std::vector<std::string>> {} = x3::string("push")   >> +space >> value;
const auto assert = x3::rule<class assert, std::vector<std::string>> {} = x3::string("assert") >> +space >> value;

const auto instr_with_operand = x3::rule<class instr_with_operand, std::vector<std::string>> {} = push | assert;
const auto instr_none_operand = x3::rule<class instr_none_operand, std::string> {} =
								x3::string("pop")
							  | x3::string("dump")
							  | x3::string("add")
							  | x3::string("sub")
							  | x3::string("mul")
							  | x3::string("div")
							  | x3::string("mod")
							  | x3::string("print")
							  | x3::string("exit");

const auto instr = x3::rule<class value, std::vector<std::string>> {} =
				 ( instr_with_operand [push_back_each]
				 | instr_none_operand [push_back]
				 ) >> *comment;

const auto sep = +x3::char_('\n');

const auto S          = x3::rule<class S,         std::vector<std::string>> {"S"}          = instr % sep;
// const auto read_file  = x3::rule<class read_file, std::vector<std::string>> {"read_file"}  = S >> "exit";
// const auto read_stdin = x3::rule<class read_file, std::vector<std::string>> {"read_stdin"} = read_file >> ";;";
// BUT actually it all should produce a vector of tokens that i'd later consume without boost::spirit

template <class Iter>
bool parse_string(Iter& begin, Iter end, std::vector<std::string>& acc)
{
	// auto parse_ok = x3::parse(begin, end, S, acc);
	auto parse_ok = x3::phrase_parse(begin, end, S, x3::char_(' ', '\t'), acc);
	if (!parse_ok)
		return false;
	return parse_ok;
}

}
