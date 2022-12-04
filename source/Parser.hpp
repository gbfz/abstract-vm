#pragma once
#include <boost/spirit/home/x3.hpp>
#include "ParserUtils.hpp"
#undef assert // not my fault it's a macro

namespace avm::parser {

namespace x3 = boost::spirit::x3;

const x3::rule<class sep> sep = "sep";
const x3::rule<class comment> comment = "comment";

const auto sep_def     = +x3::char_('\n');
const auto comment_def = *x3::space >> ';' >> *x3::char_;

const x3::rule<class N,      std::string> N = "N";
const x3::rule<class Z,      std::string> Z = "Z";
const x3::rule<class value,  std::list<std::string>> value = "value";
const x3::rule<class push,   std::list<std::string>> push = "push";
const x3::rule<class assert, std::list<std::string>> assert = "assert";
const x3::rule<class instr,  std::list<std::string>> instr = "instr";
const x3::rule<class S,      std::list<std::string>> S = "S";

const auto N_def      = -x3::char_('-') >> +x3::digit;
const auto Z_def      = -x3::char_('-') >> +x3::digit >> x3::char_('.') >> +x3::digit;

const auto value_def  = (x3::string("int8")   >> '(' >> N >> ')') [push_if_fits<int8_t>()]
					  | (x3::string("int16")  >> '(' >> N >> ')') [push_if_fits<int16_t>()]
					  | (x3::string("int32")  >> '(' >> N >> ')') [push_if_fits<int32_t>()]
					  | (x3::string("float")  >> '(' >> Z >> ')') [push_if_fits<float>()]
					  | (x3::string("double") >> '(' >> Z >> ')') [push_if_fits<double>()];

const auto push_def   = x3::string("push")   >> +x3::omit[x3::space] >> value;
const auto assert_def = x3::string("assert") >> +x3::omit[x3::space] >> value;

const auto instr_def  = (push | assert)      [push_back_each()]
					  | (x3::string("pop")
					  |  x3::string("dump")
					  |  x3::string("add")
					  |  x3::string("sub")
					  |  x3::string("mul")
					  |  x3::string("div")
					  |  x3::string("mod")
					  |  x3::string("print")
					  |  x3::string("exit")) [push_back()];

const auto S_def = -((instr >> -comment) | comment) % sep;

BOOST_SPIRIT_DEFINE(comment, sep, N, Z, value, push, assert, instr, S);

template <class Iter>
bool parse_string(Iter& begin, Iter end, std::list<std::string>& acc)
{
	auto parse_ok = x3::phrase_parse(begin, end, S, x3::char_(' ', '\t'), acc);
	if (begin != end)
		return false;
	return parse_ok;
}

template <class Iter>
bool parse_discard(Iter& begin, Iter end)
{
	auto parse_ok = x3::phrase_parse(begin, end, S, x3::char_(' ', '\t'));
	if (begin != end)
		return false;
	return parse_ok;
}

}
