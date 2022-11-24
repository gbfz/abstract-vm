#include "IOperand.hpp"
#include "MachineStack.hpp"
#include <boost/spirit/home/x3.hpp>
#include <cstdint>
#include <limits>
#include <unordered_map>

#undef assert

namespace ft::parser {

template <class Iter>
bool parse(Iter& begin, Iter end, MachineStack& s) {

	namespace x3 = boost::spirit::x3;
	using x3::digit;
	using x3::char_;

	// TODO CHECK OVERFLOW TODO
	auto create_operand = [&](std::pair<std::string, std::string>& ctx)
	{
		static const std::unordered_map<std::string, eOperandType> tm = {
			{ "int8",   eOperandType::Int8   },
			{ "int16",  eOperandType::Int16  },
			{ "int32",  eOperandType::Int32  },
			{ "float",  eOperandType::Float  },
			{ "double", eOperandType::Double },
		};
		return OperandFactory::createOperand(tm.at(ctx.first), ctx.second);
	};

	const x3::rule<struct N, std::string> N = "N";
	const auto N_def = -char_('-') >> +digit;

	const x3::rule<struct Z, std::string> Z = "Z";
	const auto Z_def = -char_('-') >> +digit >> char_('.') >> +digit;

	const x3::rule<struct sep> sep = "sep";
	const auto sep_def = char_('\n');

	const x3::rule<struct operand, std::pair<std::string, std::string>> value = "value";
	const auto value_def = (x3::string("int8")   >> "(" >> N >> ")")[create_operand]
						 | (x3::string("int16")  >> "(" >> N >> ")")[create_operand]
						 | (x3::string("int32")  >> "(" >> N >> ")")[create_operand]
						 | (x3::string("float")  >> "(" >> Z >> ")")[create_operand]
						 | (x3::string("double") >> "(" >> Z >> ")")[create_operand]
						 ;

	const x3::rule<struct comment> comment = "comment";
	const auto comment_def = char_(';') >> *char_;

	const x3::rule<struct instr> instr = "instr";
	const auto instr_def =
						(("push"   >> value)	[([&](auto&& ctx) { s.push(std::move(ctx)); })]
						|("assert" >> value)	[([&](auto&& ctx) { s.assert(std::move(ctx)); })]
						| "pop"					[([&] { s.pop(); })]
						| "dump"				[([&] { s.dump(/* TODO: where? */); })]
						| "add"					[([&] { s.add(); })]
						| "sub"					[([&] { s.sub(); })]
						| "mul"					[([&] { s.mul(); })]
						| "div"					[([&] { s.div(); })]
						| "mod"					[([&] { s.mod(); })]
						| "print"				[([&] { s.print(/* TODO: where? */); })]
						) >> *comment;
						;

	const x3::rule<struct app> app = "app";
	const auto app_def = instr >> *(sep >> instr);

	const x3::rule<struct read_file> read_file = "read_file";
	const auto read_file_def = app_def >> sep >> "exit";
	const x3::rule<struct read_stdin> read_stdin = "read_stdin";
	const auto read_stdin_def = app_def >> sep >> ";;";

	BOOST_SPIRIT_DEFINE(N, Z, sep, value, comment, instr, app, read_file, read_stdin);

}

}
