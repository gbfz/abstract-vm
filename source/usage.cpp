#include <iostream>
#include <unordered_map>

int usage()
{
	auto p = [](auto&&... args) { ((std::cout << args), ...); std::cout << "\u001b[0m"; };
	static const std::unordered_map<const char*, const char*> c = {
		{ "ornge", "\u001b[38;5;202m" },
		{ "blue", "\u001b[38;5;27m" },
		{ "reset", "\u001b[0m" },
	};
	p('\n');
	p("~ meldora's abstract-vm ~\n\n");
	p("  abstact-vm is a stack-based machine capable of performing basic arithmetic operations.\n");
	p("  now with history and tab completion!\n\n");
	p("  available numeric types:\n");
	p("  -  ", c.at("blue"), "int8\n");
	p("  -  ", c.at("blue"), "int16\n");
	p("  -  ", c.at("blue"), "int32\n");
	p("  -  ", c.at("blue"), "float\n");
	p("  -  ", c.at("blue"), "double\n\n");
	p("  here's the instruction set:\n");
	p("  -  ", c.at("ornge"), "push\n");
	p("  -  ", c.at("ornge"), "pop\n");
	p("  -  ", c.at("ornge"), "add   ", c.at("reset"), " : pop two operands, push their sum\n");
	p("  -  ", c.at("ornge"), "sub   ", c.at("reset"), " : pop two operands, push their difference\n");
	p("  -  ", c.at("ornge"), "mul   ", c.at("reset"), " : pop two operands, push their product\n");
	p("  -  ", c.at("ornge"), "div   ", c.at("reset"), " : pop two operands, push their quotient\n");
	p("  -  ", c.at("ornge"), "mod   ", c.at("reset"), " : pop two operands, push remainder of their division\n");
	p("  -  ", c.at("ornge"), "assert", c.at("reset"), " : assert that the top value on the stack matches passed parameters\n");
	p("  -  ", c.at("ornge"), "print ", c.at("reset"), " : assert that the top value is an 8-bit integer and print it as char\n");
	p("  -  ", c.at("ornge"), "dump  ", c.at("reset"), " : dump stack contents\n");
	p("  -  ", c.at("ornge"), "exit\n\n");
	p("  most instructions take no operands; ");
	p(c.at("ornge"), "push", c.at("reset"), " and ", c.at("ornge"), "assert", c.at("reset"), " take two: type and value.\n\n");
	p("       instr    type(value)\n");
	p("  e.g. ", c.at("ornge"), "push  ", c.at("blue"), " double", c.at("reset"), "(42.42)\n");
	p("       ", c.at("ornge"), "assert", c.at("blue"), " double", c.at("reset"), "(42.42)\n");
	p("       ", c.at("ornge"), "push ", c.at("blue"), "    int8", c.at("reset"), "(109)\n");
	p("       ", c.at("ornge"), "print\n");
	p("       ", c.at("ornge"), "exit\n\n");
	p("  avm supports reading from stdin and from a file passed to the executable.\n");
	p("  to end input, write ", c.at("ornge"), "'exit'");
	p("; on stdin also enter ", c.at("ornge"), "';;'", c.at("reset"), ".\n\n");
	return 0;
}
