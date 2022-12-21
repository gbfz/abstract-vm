#include "MachineStack.hpp"
#include "Input.hpp"
#include "eOperandType.hpp"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace avm {

auto getTwo(std::list<std::string>& tokens)
{
	auto type = avm::toEnum(std::move(tokens.front()));
	tokens.pop_front();
	auto value = std::move(tokens.front());
	tokens.pop_front();
	return std::make_pair(std::move(type), std::move(value));
};

auto push(MachineStack& stack, std::list<std::string>& tokens)
{
	auto [type, value] = getTwo(tokens);
	stack.push(OperandFactory::createOperand(type, std::move(value)));
}

auto assert(MachineStack& stack, std::list<std::string>& tokens)
{
	auto [type, value] = getTwo(tokens);
	stack.assert(type, std::move(value));
}

auto save(MachineStack& stack, std::list<std::string>& tokens)
{
	auto reg_name = std::move(tokens.front());
	tokens.pop_front();
	stack.save(std::move(reg_name));
}

auto load(MachineStack& stack, std::list<std::string>& tokens)
{
	auto reg_name = std::move(tokens.front());
	tokens.pop_front();
	stack.load(std::move(reg_name));
}

void exec(std::list<std::string>& tokens)
{
	MachineStack stack;

	std::unordered_map<std::string, std::function<void()>> handlers =
	{
		{ "push",   [&] { push  (stack, tokens); } },
		{ "assert", [&] { assert(stack, tokens); } },
		{ "save",   [&] { save  (stack, tokens); } },
		{ "load",   [&] { load  (stack, tokens); } },
		{ "print",  [&] { stack.print(); } },
		{ "dump",   [&] { stack.dump(); } },
		{ "pop",    [&] { stack.pop(); } },
		{ "add",    [&] { stack.add(); } },
		{ "sub",    [&] { stack.sub(); } },
		{ "mul",    [&] { stack.mul(); } },
		{ "div",    [&] { stack.div(); } },
		{ "mod",    [&] { stack.mod(); } },
		{ "dup",    [&] { stack.dup(); } },
		{ "exit",   [ ] { } }
	};

	while (!tokens.empty())
	{
		auto instruction = std::move(tokens.front());
		tokens.pop_front();
		handlers.at(instruction)();
	}
}

int work()
{
	try {
		auto token_stream = avm::readInput();
		exec(token_stream);
	} catch (std::exception& e) {
		std::cout << "fatal error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}

int work(char* filename)
{
	try {
		std::ifstream file { filename };
		if (!file.is_open() || file.bad())
			throw std::invalid_argument("Cannot open or read given file");
		auto token_stream = avm::readInput(std::move(file));
		exec(token_stream);
	} catch (std::exception& e) {
		std::cout << "fatal error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}

} // namespace avm

int usage();

int main(int ac, char** av)
{
	if (ac == 1)
		return avm::work();
	if (ac == 2)
	{
		std::string arg = av[1];
		if (arg == "-h" || arg == "--help")
			return usage();
	   	return avm::work(av[1]);
	}
	return usage();
}
