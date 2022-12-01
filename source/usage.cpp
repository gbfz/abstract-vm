#include <iostream>
#include <unordered_map>

void usage()
{
	auto p = [](auto&&... args) { ((std::cout << args), ...); std::cout << "\u001b[0m"; };
	static const std::unordered_map<const char*, const char*> c = {
		{ "orng", "\u001b[38;5;202m" },
		{ "wht", "\u001b[38;5;251m" },
		{ "blu", "\u001b[38;5;27m" },
		{ "rst", "\u001b[0m" },
	};
	p('\n');
	p("~ meldora's abstract-vm usage ~\n\n");
	p("  avm is a stack-based machine capable of performing basic arithmetic operations.\n\n");
	p("  available numeric types:\n");
	p("  -  ", c.at("blu"), "int8\n");
	p("  -  ", c.at("blu"), "int16\n");
	p("  -  ", c.at("blu"), "int32\n");
	p("  -  ", c.at("blu"), "float\n");
	p("  -  ", c.at("blu"), "double\n\n");
	p("  here's the instruction set:\n");
	p("  -  ", c.at("orng"), "push\n");
	p("  -  ", c.at("orng"), "pop\n");
	p("  -  ", c.at("orng"), "add   ", c.at("rst"), " : pop two operands, push their sum\n");
	p("  -  ", c.at("orng"), "sub   ", c.at("rst"), " : pop two operands, push their difference\n");
	p("  -  ", c.at("orng"), "mul   ", c.at("rst"), " : pop two operands, push their product\n");
	p("  -  ", c.at("orng"), "div   ", c.at("rst"), " : pop two operands, push their quotient\n");
	p("  -  ", c.at("orng"), "mod   ", c.at("rst"), " : pop two operands, push remainder of their division\n");
	p("  -  ", c.at("orng"), "assert", c.at("rst"), " : assert that the top value on the stack matches passed parameters\n");
	p("  -  ", c.at("orng"), "print ", c.at("rst"), " : assert that the top value is an 8-bit integer and print it as char\n");
	p("  -  ", c.at("orng"), "dump  ", c.at("rst"), " : dump the stack's contents\n");
	p("  -  ", c.at("orng"), "exit\n\n");
	p("  most instructions take no operands; ");
	p(c.at("orng"), "push", c.at("wht"), " and ", c.at("orng"), "assert", c.at("wht"), " take two operands: type and value.\n\n");
	p("       <instr> <type>(<value>)\n");
	p("  e.g. ", c.at("orng"), "push ", c.at("blu"), "   double", c.at("wht"), "(42.42)\n");
	p("       ", c.at("orng"), "push ", c.at("blu"), "     int8", c.at("wht"), "(97)\n");
	p("       ", c.at("orng"), "print\n");
	p("       ", c.at("orng"), "exit\n\n");
	p("  avm supports reading from stdin and from a file passed to the executable.\n");
	p("  to end input, write ", c.at("orng"), "'exit'");
	p("; on stdin follow that by a ", c.at("orng"), "';;'", c.at("wht"), ".\n\n");
}
