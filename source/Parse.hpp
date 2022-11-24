#include "IOperand.hpp"
#include "MachineStack.hpp"
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/char/char_class.hpp>
#include <boost/spirit/home/x3/core/call.hpp>
#include <boost/spirit/home/x3/core/parse.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/operator/detail/sequence.hpp>
#include <unordered_map>
#include <vector>

BOOST_FUSION_ADAPT_STRUCT(MachineStack,
	(std::vector<std::unique_ptr<const IOperand>>, values)
)

#undef assert

namespace ft::parser {

namespace x3 = boost::spirit::x3;
using x3::digit;
using x3::char_;

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

// TODO CHECK OVERFLOW TODO
static auto create_operand = [](auto&& ctx)
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

template <typename T> static auto as = [](auto p) { return x3::rule<struct tag, T> {"as"} = p; };

// const auto N = x3::rule<class N, std::string> {"N"} = as<std::string>(-char_('-') >> +digit);
// const auto Z = x3::rule<class Z, std::string> {"Z"} = as<std::string>(-char_('-') >> +digit >> char_('.') >> +digit);

const auto value = x3::rule<class value, std::vector<std::string>> {"value"} = as<std::vector<std::string>>
				 (  (x3::string("int8")   >> "(" >> "0" >> ")")
				  | (x3::string("int16")  >> "(" >> "0" >> ")")
				  | (x3::string("int32")  >> "(" >> "0" >> ")")
				  | (x3::string("float")  >> "(" >> "0" >> ")")
				  | (x3::string("double") >> "(" >> "0" >> ")")
				 );

static auto _push	= [](auto&& ctx) { x3::_val(ctx).push(create_operand(ctx)); };
static auto _assert	= [](auto&& ctx) { x3::_val(ctx).assert(create_operand(ctx)); };
static auto _pop	= [](auto&& ctx) { x3::_val(ctx).pop(); };
static auto _dump	= [](auto&& ctx) { x3::_val(ctx).dump(/* TODO: where? */); };
static auto _add	= [](auto&& ctx) { x3::_val(ctx).add(); };
static auto _sub	= [](auto&& ctx) { x3::_val(ctx).sub(); };
static auto _mul	= [](auto&& ctx) { x3::_val(ctx).sub(); };
static auto _div	= [](auto&& ctx) { x3::_val(ctx).div(); };
static auto _mod	= [](auto&& ctx) { x3::_val(ctx).mod(); };
static auto _print	= [](auto&& ctx) { x3::_val(ctx).print(/* TODO: where? */); };

const auto instr = x3::rule<class instr, MachineStack> {"instr"} = as<MachineStack>
				 (( ("push"   >> value)	[_push]
 				  | ("assert" >> value)	[_assert]
 				  | x3::lit("pop")		[_pop]
 				  | x3::lit("dump")  	[_dump]
 				  | x3::lit("add")		[_add]
 				  | x3::lit("sub")		[_sub]
 				  | x3::lit("mul")		[_mul]
 				  | x3::lit("div")		[_div]
 				  | x3::lit("mod")		[_mod]
 				  | x3::lit("print")	[_print]
 				  ) >> *(char_(';') >> *char_)
				 );

const auto app		  = x3::rule<class app, MachineStack> {"app"} 				= as<MachineStack> (instr >> *("\n" >> instr));
const auto read_file  = x3::rule<class read_file, MachineStack> {"read_file"}	= as<MachineStack> (app >> "\n" >> "exit");
const auto read_stdin = x3::rule<class read_stdin, MachineStack> {"read_stdin"}	= as<MachineStack> (app >> "\n" >> ";;");


template <class Iter>
bool parse_assembly(Iter& begin, Iter end, MachineStack& s)
{
	bool b = x3::phrase_parse(begin, end, read_file, x3::space, s);
	if (begin != end)
		return false;
	return b;
}

}
