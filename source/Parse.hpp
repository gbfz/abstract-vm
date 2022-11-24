#include "MachineStack.hpp"
#include <boost/spirit/home/x3.hpp>
#include <unordered_map>
#include <vector>
#undef assert

namespace ft::parser {

namespace x3 = boost::spirit::x3;

template <class Iter>
bool parse(Iter& begin, Iter end, MachineStack& stack, bool isatty = false)
{
	// TODO check overflow
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
	auto _push	 = [&](auto&& ctx) { stack.push(create_operand(ctx)); };
	auto _assert = [&](auto&& ctx) { stack.assert(create_operand(ctx)); };
	auto _pop	 = [&] { stack.pop(); };
	auto _dump	 = [&] { stack.dump(/* where? */); };
	auto _add	 = [&] { stack.add(); };
	auto _sub	 = [&] { stack.sub(); };
	auto _mul	 = [&] { stack.sub(); };
	auto _div	 = [&] { stack.div(); };
	auto _mod	 = [&] { stack.mod(); };
	auto _print	 = [&] { stack.print(/* where? */); };

	const auto N = x3::rule<class N, std::string> {"N"} = -x3::char_('-') >> +x3::digit;
	const auto Z = x3::rule<class Z, std::string> {"Z"} = -x3::char_('-') >> +x3::digit >> '.' >> +x3::digit;

	const auto value = x3::rule<class value, std::vector<std::string>> {"value"} =
					  (x3::string("int8")   >> "(" >> N >> ")")
					| (x3::string("int16")  >> "(" >> N >> ")")
					| (x3::string("int32")  >> "(" >> N >> ")")
					| (x3::string("float")  >> "(" >> Z >> ")")
					| (x3::string("double") >> "(" >> Z >> ")");

	const auto instr = ( ("push"   >> value)	[_push]
					   | ("assert" >> value)	[_assert]
					   | x3::lit("pop")			[_pop]
					   | x3::lit("dump")  		[_dump]
					   | x3::lit("add")			[_add]
					   | x3::lit("sub")			[_sub]
					   | x3::lit("mul")			[_mul]
					   | x3::lit("div")			[_div]
					   | x3::lit("mod")			[_mod]
					   | x3::lit("print")		[_print]
					   ) >> *(';' >> *x3::char_);

	const auto sep		  = +x3::char_('\n');
	const auto app		  = instr >> *(sep >> instr);
	const auto read_file  = +app >> "exit";

	bool b = x3::phrase_parse(begin, end, read_file, x3::space);
	if (!b)
	// if (begin != end)
		return false;
	return b;
}

}
