#include <iostream>
#include <unordered_map>

int usage()
{
	auto p = [](auto&&... args) { ((std::cout << args), ...); std::cout << "\u001b[0m"; };
	static const std::unordered_map<const char*, const char*> c = {
		{ "orng", "\u001b[38;5;202m" },
		{ "whit", "\u001b[38;5;251m" },
		{ "blue", "\u001b[38;5;27m" },
		{ "rset", "\u001b[0m" },
	};
	p('\n');
	p("~ meldora's abstract-vm usage ~\n\n");
	p("  avm is a stack-based machine capable of performing basic arithmetic operations.\n\n");
	p("  available numeric types:\n");
	p("  -  ", c.at("blue"), "int8\n");
	p("  -  ", c.at("blue"), "int16\n");
	p("  -  ", c.at("blue"), "int32\n");
	p("  -  ", c.at("blue"), "float\n");
	p("  -  ", c.at("blue"), "double\n\n");
	p("  here's the instruction set:\n");
	p("  -  ", c.at("orng"), "push\n");
	p("  -  ", c.at("orng"), "pop\n");
	p("  -  ", c.at("orng"), "add   ", c.at("rset"), " : pop two operands, push their sum\n");
	p("  -  ", c.at("orng"), "sub   ", c.at("rset"), " : pop two operands, push their difference\n");
	p("  -  ", c.at("orng"), "mul   ", c.at("rset"), " : pop two operands, push their product\n");
	p("  -  ", c.at("orng"), "div   ", c.at("rset"), " : pop two operands, push their quotient\n");
	p("  -  ", c.at("orng"), "mod   ", c.at("rset"), " : pop two operands, push remainder of their division\n");
	p("  -  ", c.at("orng"), "assert", c.at("rset"), " : assert that the top value on the stack matches passed parameters\n");
	p("  -  ", c.at("orng"), "print ", c.at("rset"), " : assert that the top value is an 8-bit integer and print it as char\n");
	p("  -  ", c.at("orng"), "dump  ", c.at("rset"), " : dump the stack's contents\n");
	p("  -  ", c.at("orng"), "exit\n\n");
	p("  most instructions take no operands; ");
	p(c.at("orng"), "push", c.at("whit"), " and ", c.at("orng"), "assert", c.at("wht"), " take two operands: type and value.\n\n");
	p("       <instr> <type>(<value>)\n");
	p("  e.g. ", c.at("orng"), "push ", c.at("blue"), "   double", c.at("whit"), "(42.42)\n");
	p("       ", c.at("orng"), "push ", c.at("blue"), "     int8", c.at("whit"), "(97)\n");
	p("       ", c.at("orng"), "print\n");
	p("       ", c.at("orng"), "exit\n\n");
	p("  avm supports reading from stdin and from a file passed to the executable.\n");
	p("  to end input, write ", c.at("orng"), "'exit'");
	p("; on stdin follow that by a ", c.at("orng"), "';;'", c.at("whit"), ".\n\n");
	return 0;
}
