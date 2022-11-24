#include "IOperand.hpp"
#include "MachineStack.hpp"
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/char/char_class.hpp>
#include <boost/spirit/home/x3/core/call.hpp>
#include <boost/spirit/home/x3/core/parse.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <functional>
#include <unordered_map>

BOOST_FUSION_ADAPT_STRUCT(MachineStack,
(std::vector<std::unique_ptr<const IOperand>>, values)
)

#undef assert

namespace ft::parser {

namespace x3 = boost::spirit::x3;
using x3::digit;
using x3::char_;

/*
auto create_operand(std::pair<std::string, std::string>& ctx)
{
	static const std::unordered_map<std::string, eOperandType> tm = {
		{ "int8",   eOperandType::Int8   },
		{ "int16",  eOperandType::Int16  },
		{ "int32",  eOperandType::Int32  },
		{ "float",  eOperandType::Float  },
		{ "double", eOperandType::Double },
	};
	return OperandFactory::createOperand(tm.at(ctx.first), ctx.second);
}
*/

// TODO CHECK OVERFLOW TODO
static auto create_operand = [](auto& ctx)
{
	static const std::unordered_map<std::string, eOperandType> tm = {
		{ "int8",   eOperandType::Int8   },
		{ "int16",  eOperandType::Int16  },
		{ "int32",  eOperandType::Int32  },
		{ "float",  eOperandType::Float  },
		{ "double", eOperandType::Double },
	};
	eOperandType t = tm.at(x3::_attr(ctx).front());
	std::string  v = std::move(x3::_attr(ctx).back());
	x3::_val(ctx) = OperandFactory::createOperand(t, v);
};

/*
auto create_operand(auto& ctx)
{
	static const std::unordered_map<std::string, eOperandType> tm = {
		{ "int8",   eOperandType::Int8   },
		{ "int16",  eOperandType::Int16  },
		{ "int32",  eOperandType::Int32  },
		{ "float",  eOperandType::Float  },
		{ "double", eOperandType::Double },
	};
	eOperandType t = tm.at(x3::_attr(ctx).front());
	std::string  v = std::move(x3::_attr(ctx).back());
	x3::_val(ctx) = OperandFactory::createOperand(t, v);
};
*/

template <typename T> static auto as = [](auto p) { return x3::rule<struct tag, T> {"as"} = p; };

const auto N       = x3::rule<class N, std::string> {"N"}
			       = as<std::string>
			       (-char_('-') >> +digit);
const auto Z       = x3::rule<class Z, std::string> {"Z"}
			       = as<std::string>
			       (-char_('-') >> +digit >> char_('.') >> +digit);
const auto sep     = x3::rule<class sep, x3::unused_type> {"sep"}
			       = as<x3::unused_type>
			       (char_('\n'));
const auto value   = x3::rule<class value, std::unique_ptr<const IOperand>> {"value"}
				   = as<std::unique_ptr<const IOperand>>
				    ( (x3::string("int8") >> "(" >> N >> ")")   [create_operand]
				  	| (x3::string("int16") >> "(" >> N >> ")")  [create_operand]
				  	| (x3::string("int32") >> "(" >> N >> ")")  [create_operand]
				  	| (x3::string("float") >> "(" >> Z >> ")")  [create_operand]
				  	| (x3::string("double") >> "(" >> Z >> ")") [create_operand]
				    );
const auto comment = x3::rule<class comment, x3::unused_type> {"comment"}
				   = as<x3::unused_type>
				   (char_(';') >> *char_);

const auto instr   = x3::rule<class instr, MachineStack> {"instr"}
				   = as<MachineStack>
				    (( ("push"   >> value)	[([](auto&& ctx) { x3::_val(ctx).push(std::move(x3::_attr(ctx))); })]
 					 | ("assert" >> value)	[([](auto&& ctx) { x3::_val(ctx).assert(std::move(x3::_attr(ctx))); })]
 					 |  x3::lit("pop")		[([](auto&& ctx) { x3::_val(ctx).pop(); })]
 					 |  x3::lit("dump")  	[([](auto&& ctx) { x3::_val(ctx).dump(/* TODO: where? */); })]
 					 |  x3::lit("add")		[([](auto&& ctx) { x3::_val(ctx).add(); })]
 					 |  x3::lit("sub")		[([](auto&& ctx) { x3::_val(ctx).sub(); })]
 					 |  x3::lit("mul")		[([](auto&& ctx) { x3::_val(ctx).mul(); })]
 					 |  x3::lit("div")		[([](auto&& ctx) { x3::_val(ctx).div(); })]
 					 |  x3::lit("mod")		[([](auto&& ctx) { x3::_val(ctx).mod(); })]
 					 |  x3::lit("print")	[([](auto&& ctx) { x3::_val(ctx).print(/* TODO: where? */); })]
 					 ) >> *comment
					);

const auto app = x3::rule<class instr, MachineStack> {"app"}
			   = as<MachineStack>
			   (instr >> *(sep >> instr));

const auto read_file = x3::rule<class read_file, MachineStack> {"read_file"}
					 = as<MachineStack>
					 (app >> sep >> "exit");

const auto read_stdin = x3::rule<class read_stdin, MachineStack> {"read_stdin"}
					  = as<MachineStack>
					  (app >> sep >> ";;");

/*
const x3::rule<struct N, std::string> N = "N";
const auto N_def = -char_('-') >> +digit;

const x3::rule<struct Z, std::string> Z = "Z";
const auto Z_def = -char_('-') >> +digit >> char_('.') >> +digit;

const x3::rule<struct sep> sep = "sep";
const auto sep_def = char_('\n');

const x3::rule<struct operand, std::pair<std::string, std::string>> value = "value";
const auto value_def = (x3::string("int8")   >> "(" >> N >> ")") [create_operand]
					 | (x3::string("int16")  >> "(" >> N >> ")") [create_operand]
					 | (x3::string("int32")  >> "(" >> N >> ")") [create_operand]
					 | (x3::string("float")  >> "(" >> Z >> ")") [create_operand]
					 | (x3::string("double") >> "(" >> Z >> ")") [create_operand]
					 ;

const x3::rule<struct comment> comment = "comment";
const auto comment_def = char_(';') >> *char_;
*/

// const x3::rule<struct instr, MachineStack> instr = "instr";
// const auto instr_def =
// 					(("push"   >> value)	[([](auto&& ctx) { x3::_val(ctx).push(std::move(ctx)); })]
// 					|("assert" >> value)	[([](auto&& ctx) { x3::_val(ctx).assert(std::move(ctx)); })]
// 					| x3::lit("pop")		[([](auto&& ctx) { x3::_val(ctx).pop(); })]
// 					| x3::lit("dump")  		[([](auto&& ctx) { x3::_val(ctx).dump(/* TODO: where? */); })]
// 					| x3::lit("add")  		[([](auto&& ctx) { x3::_val(ctx).add(); })]
// 					| x3::lit("sub")  		[([](auto&& ctx) { x3::_val(ctx).sub(); })]
// 					| x3::lit("mul")  		[([](auto&& ctx) { x3::_val(ctx).mul(); })]
// 					| x3::lit("div")  		[([](auto&& ctx) { x3::_val(ctx).div(); })]
// 					| x3::lit("mod")  		[([](auto&& ctx) { x3::_val(ctx).mod(); })]
// 					| x3::lit("print")		[([](auto&& ctx) { x3::_val(ctx).print(/* TODO: where? */); })]
// 					) >> *comment;
// 					;

/*
const x3::rule<struct app, MachineStack> app = "app";
const auto app_def = instr >> *(sep >> instr);

const x3::rule<struct read_file, MachineStack> read_file = "read_file";
const auto read_file_def = app_def >> sep >> "exit";
const x3::rule<struct read_stdin, MachineStack> read_stdin = "read_stdin";
const auto read_stdin_def = app_def >> sep >> ";;";

BOOST_SPIRIT_DEFINE(N, Z, sep, value, comment, instr, app, read_file, read_stdin);
*/

template <class Iter>
bool parse_assembly(Iter& begin, Iter end, MachineStack& s)
{
	bool b = x3::phrase_parse(begin, end, read_file, x3::space, s);
	if (begin != end)
		return false;
	return b;
}

}
