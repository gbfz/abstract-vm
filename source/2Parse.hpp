#pragma once
#include <boost/spirit/home/x3.hpp>
#include "ParserUtils.hpp"
#include <vector>
#include <limits>
#undef assert // not my fault it's a macro

namespace avm::parser {

namespace x3 = boost::spirit::x3;

const auto N = make_rule<std::string>() = -x3::char_('-') >> +x3::digit;
const auto Z = make_rule<std::string>() = -x3::char_('-') >> +x3::digit >> '.' >> +x3::digit;

const auto value = make_rule<std::vector<std::string>>() =
				  (x3::string("int8")   >> '(' >> N >> ')') [push_if_fits<int8_t>()]
				| (x3::string("int16")  >> '(' >> N >> ')') [push_if_fits<int16_t>()]
				| (x3::string("int32")  >> '(' >> N >> ')') [push_if_fits<int32_t>()]
				| (x3::string("float")  >> '(' >> Z >> ')') [push_if_fits<float>()]
				| (x3::string("double") >> '(' >> Z >> ')') [push_if_fits<double>()];

const auto space = x3::lit(' ') | x3::lit('\t');

const auto push   = make_rule<std::vector<std::string>>() = x3::string("push")   >> +space >> value;
const auto assert = make_rule<std::vector<std::string>>() = x3::string("assert") >> +space >> value;

const auto instr_with_operand = make_rule<std::vector<std::string>>() = push | assert;
const auto instr_none_operand = make_rule<std::string>() =
								x3::string("pop") | x3::string("dump")
							  | x3::string("add") | x3::string("sub")
							  | x3::string("mul") | x3::string("div")
							  | x3::string("mod") | x3::string("print")
							  | x3::string("exit");

const auto comment = ';' >> -(x3::char_ - ';') >> *x3::char_;

const auto instr = make_rule<std::vector<std::string>>() =
				 ( instr_with_operand [push_back_each()]
				 | instr_none_operand [push_back()]
				 ) >> *comment;

const auto sep = +x3::char_('\n');

const auto S          = make_rule<std::vector<std::string>>()          = instr % sep;
// const auto read_file  = x3::rule<class read_file, std::vector<std::string>> {"read_file"}  = S >> "exit";
// const auto read_stdin = x3::rule<class read_file, std::vector<std::string>> {"read_stdin"} = read_file >> ";;";
// BUT actually it all should produce a vector of tokens that i'd later consume without boost::spirit

template <class Iter>
bool parse_string(Iter& begin, Iter end, std::vector<std::string>& acc)
{
	// auto parse_ok = x3::parse(begin, end, S, acc);
	auto parse_ok = x3::phrase_parse(begin, end, S, x3::char_(' ', '\t'), acc);
	if (begin != end)
		return false;
	return parse_ok;
}

}
