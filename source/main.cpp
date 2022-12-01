#include "MachineStack.hpp"
#include "OperandFactory.hpp"
#include "eOperandType.hpp"
#include "InputLoop.hpp"
#include <fstream>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <exception>
#include <list>
#include <iostream>

eOperandType toEnum(const std::string& t)
{
	static const std::unordered_map<std::string, eOperandType> type = {
		{ "int8",   eOperandType::Int8   },
		{ "int16",  eOperandType::Int16  },
		{ "int32",  eOperandType::Int32  },
		{ "float",  eOperandType::Float  },
		{ "double", eOperandType::Double },
	};
	return type.at(t);
}

auto getTwo(std::list<std::string>& tokens)
{
	auto type = toEnum(std::move(tokens.front()));
	tokens.pop_front();
	auto value = std::move(tokens.front());
	tokens.pop_front();
	return std::make_pair(type, value);
};

auto _push(MachineStack& stack, std::list<std::string>& tokens)
{
	auto [type, value] = getTwo(tokens);
	stack.push(OperandFactory::createOperand(type, std::move(value)));
}

auto _assert(MachineStack& stack, std::list<std::string>& tokens)
{
	auto [type, value] = getTwo(tokens);
	stack.assert(type, std::move(value));
}

void exec(std::list<std::string>& tokens)
{
	MachineStack stack;

	std::unordered_map<std::string, std::function<void()>> handlers =
	{
		{ "push",   [&] { _push  (stack, tokens); } },
		{ "assert", [&] { _assert(stack, tokens); } },
		{ "print",  [&] { stack.print(); } },
		{ "dump",   [&] { stack.dump(); } },
		{ "pop",    [&] { stack.pop(); } },
		{ "add",    [&] { stack.add(); } },
		{ "sub",    [&] { stack.sub(); } },
		{ "mul",    [&] { stack.mul(); } },
		{ "div",    [&] { stack.div(); } },
		{ "mod",    [&] { stack.mod(); } },
		{ "exit",   [ ] { } }
	};

	while (!tokens.empty())
	{
		auto instruction = std::move(tokens.front());
		tokens.pop_front();
		handlers.at(instruction)();
	}
}

void work()
{
	try {
		auto token_stream = avm::readInput();
		exec(token_stream);
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
		exit(21);
	}
}

void work(char* filename)
{
	try {
		std::ifstream file { filename };
		if (!file.is_open() || file.bad())
			throw std::invalid_argument("Cannot open or read given file");
		auto token_stream = avm::readInput(std::move(file));
		exec(token_stream);
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
		exit(21);
	}
}

void usage();

int main(int ac, char** av)
{
	if (ac == 1)
		work();
	else if (ac == 2)
	{
		std::string arg = av[1];
		if (arg == "-h" || arg == "--help")
		{
			usage();
			return 0;
		}
	   	work(av[1]);
	}
	else usage();
}
