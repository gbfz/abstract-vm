#include "VirtualMachine.hpp"
#include "Input.hpp"
#include <exception>
#include <iostream>

namespace avm {

void VirtualMachine::exec_internal(const char* stream_name)
{
	if (stream_name == ""s)
		tokens = read_from_stdin();
	else tokens = read_from_file(stream_name);
	while (!tokens.empty())
	{
		auto instruction = std::move(tokens.front());
		tokens.pop_front();
		handlers.at(instruction)();
	}
}

int VirtualMachine::exec(const char* stream_name)
{
	try {
		exec_internal(stream_name);
	} catch (std::exception& e) {
		std::cout << "fatal error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}

std::list<std::string> VirtualMachine::read_from_stdin()
{
	return avm::readInput();
}

std::list<std::string> VirtualMachine::read_from_file(const char* filename)
{
	std::ifstream file { filename };
	if (!file.is_open() || file.bad())
		throw std::invalid_argument("cannot open or read given file");
	return avm::readInput(std::move(file));
}

int VirtualMachine::usage() const
{
	auto p = [](auto&&... args) { ((std::cout << args), ...); std::cout << "\u001b[0m"; };
	static const std::unordered_map<const char*, const char*> c = {
		{ "orange", "\u001b[38;5;202m" },
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
	p("  -  ", c.at("orange"), "push   ", c.at("reset"), "type(value)\n");
	p("  -  ", c.at("orange"), "assert ", c.at("reset"), "type(value)\n");
	p("  -  ", c.at("orange"), "pop\n");
	p("  -  ", c.at("orange"), "save   ", c.at("reset"), ": pop value and save it to a register (A or B)\n");
	p("  -  ", c.at("orange"), "load   ", c.at("reset"), ": push value from register (A or B) on stack\n");
	p("  -  ", c.at("orange"), "add   ", c.at("reset"), " : pop two operands, push their sum\n");
	p("  -  ", c.at("orange"), "sub   ", c.at("reset"), " : pop two operands, push their difference\n");
	p("  -  ", c.at("orange"), "mul   ", c.at("reset"), " : pop two operands, push their product\n");
	p("  -  ", c.at("orange"), "div   ", c.at("reset"), " : pop two operands, push their quotient\n");
	p("  -  ", c.at("orange"), "mod   ", c.at("reset"), " : pop two operands, push remainder of their division\n");
	p("  -  ", c.at("orange"), "dup   ", c.at("reset"), " : duplicate value on top of stack\n");
	p("  -  ", c.at("orange"), "print ", c.at("reset"), " : assert that the top value is an 8-bit integer and print it as char\n");
	p("  -  ", c.at("orange"), "dump  ", c.at("reset"), " : dump stack content\n");
	p("  -  ", c.at("orange"), "exit\n\n");
	p("  most instructions take no operands; ");
	p(c.at("orange"), "push", c.at("reset"), " and ", c.at("orange"), "assert", c.at("reset"), " take two: type and value.\n\n");
	p("  e.g. ", c.at("orange"), "push ", c.at("blue"), "    int8", c.at("reset"), "(109)\n");
	p("       ", c.at("orange"), "print\n");
	p("       ", c.at("orange"), "push  ", c.at("blue"), " double", c.at("reset"), "(42.42)\n");
	p("       ", c.at("orange"), "assert", c.at("blue"), " double", c.at("reset"), "(42.42)\n");
	p("       ", c.at("orange"), "exit\n\n");
	p("  avm supports reading from stdin and from a file passed to the executable.\n");
	p("  to end input, write ", c.at("orange"), "'exit'");
	p("; on stdin also enter ", c.at("orange"), "';;'", c.at("reset"), ".\n\n");
	return 0;
}

}
