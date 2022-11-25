#pragma once
#include <boost/spirit/home/x3.hpp>
#include "Token.hpp"
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <unordered_map>
#include <vector>
#undef assert // not my fault it's a macro

BOOST_FUSION_ADAPT_STRUCT(token,
	(eOperandType, type),
	(std::unique_ptr<const IOperand>, value)
)

namespace avm::parser {

namespace x3 = boost::spirit::x3;

const auto N = x3::rule<class N, std::string> {"N"} = -x3::char_('-') >> +x3::digit;
const auto Z = x3::rule<class Z, std::string> {"Z"} = -x3::char_('-') >> +x3::digit >> '.' >> +x3::digit;

const auto value = x3::rule<class value, std::string> {"value"} =
				  (x3::string("int8")   >> "(" >> N >> ")")
				| (x3::string("int16")  >> "(" >> N >> ")")
				| (x3::string("int32")  >> "(" >> N >> ")")
				| (x3::string("float")  >> "(" >> Z >> ")")
				| (x3::string("double") >> "(" >> Z >> ")");

const auto comment = ';' >> -(x3::char_ - ';') >> *x3::char_;

const auto instr = x3::rule<class value, std::string> {"instr"} =
				( (x3::string("push")   >> +x3::space >> value)
				| (x3::string("assert") >> +x3::space >> value)
				|  x3::string("pop")
				|  x3::string("dump")
				|  x3::string("add")
				|  x3::string("sub")
				|  x3::string("mul")
				|  x3::string("div")
				|  x3::string("mod")
				|  x3::string("print")
				|  x3::string("exit")
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
	if (begin != end)
		return false;
	return parse_ok;
}

}
