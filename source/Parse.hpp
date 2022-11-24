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
#include <boost/spirit/home/x3/nonterminal/detail/rule.hpp>
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

template <class Iter>
bool parse(Iter& begin, Iter end, MachineStack& stack)
{

	// TODO CHECK OVERFLOW TODO
	auto create_operand = [](auto&& ctx)
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
		return OperandFactory::createOperand(t, std::move(v));
	};

	const auto N = x3::rule<class N, std::string> {"N"} = (-char_('-') >> +digit);
	const auto Z = x3::rule<class Z, std::string> {"Z"} = (-char_('-') >> +digit >> char_('.') >> +digit);

	const auto value = x3::rule<class value, std::vector<std::string>> {"value"} =
					 (  (x3::string("int8")   >> "(" >> N >> ")")
					  | (x3::string("int16")  >> "(" >> N >> ")")
					  | (x3::string("int32")  >> "(" >> N >> ")")
					  | (x3::string("float")  >> "(" >> Z >> ")")
					  | (x3::string("double") >> "(" >> Z >> ")")
					 );

	static auto _push	= [&](auto&& ctx) { stack.push(create_operand(ctx)); };
	static auto _assert	= [&](auto&& ctx) { stack.assert(create_operand(ctx)); };
	static auto _pop	= [&stack]() { stack.pop(); };
	static auto _dump	= [&stack]() { stack.dump(/* TODO: where? */); };
	static auto _add	= [&stack]() { stack.add(); };
	static auto _sub	= [&stack]() { stack.sub(); };
	static auto _mul	= [&stack]() { stack.sub(); };
	static auto _div	= [&stack]() { stack.div(); };
	static auto _mod	= [&stack]() { stack.mod(); };
	static auto _print	= [&stack]() { stack.print(/* TODO: where? */); };

	const auto instr = x3::rule<class instr> {"instr"} =
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

	const auto app		  = x3::rule<class app> {"app"} 			  = (instr >> *("\n" >> instr));
	const auto read_file  = x3::rule<class read_file> {"read_file"}	  = (app >> "\n" >> "exit");
	const auto read_stdin = x3::rule<class read_stdin> {"read_stdin"} = (app >> "\n" >> ";;");

	bool b = x3::phrase_parse(begin, end, read_file, x3::space);
	if (begin != end)
		return false;
	return b;
}

}
