#include "IOperand.hpp"
#include "Operand.hpp"
#include "MachineStack.hpp"
#include "2Parse.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

void testParse()
{
	std::string input = "add\n\n\npush int8(0)\ndump";
	// std::string input = "dump\ndump";
	auto begin = input.begin(), end = input.end();
	std::vector<std::string> acc;
	auto parse_ok = avm::parser::parse_string(begin, end, acc);
	if (!parse_ok)
		std::cout << "Error at: " << std::string(begin, end) << '\n';
	else for (auto&& s : acc)
		std::cout << s << '\n';
}

void testLoop()
{
	std::string buf;
	std::vector<std::string> acc;
	while (std::getline(std::cin, buf))
	{
		auto begin = buf.begin(), end = buf.end();
		bool parse_ok = avm::parser::parse_string(begin, end, acc);
		if (!parse_ok)
		{
			std::cout << "Error at: " << std::string(begin, end) << '\n';
			exit(21);
		}
	}
	for (auto&& s : acc)
		std::cout << s << '\n';
}

void testStream()
{
	std::stringstream sstream;
	std::string buf;
	while (std::getline(std::cin, buf))
		sstream << buf << '\n';
	std::vector<std::string> acc;
	while (std::getline(sstream, buf))
	{
		auto begin = buf.begin(), end = buf.end();
		bool parse_ok = avm::parser::parse_string(begin, end, acc);
		if (!parse_ok)
		{
			std::cout << "Error at: " << std::string(begin, end) << '\n';
			exit(21);
		}
	}
	for (auto&& s : acc)
		std::cout << s << '\n';
}

int main()
{
	// testMachine();
	// test2();
	// test3();
	// testParse();
	testLoop();
	// testStream();
}
