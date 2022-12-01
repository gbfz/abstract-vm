#include "IOperand.hpp"
#include "Operand.hpp"
#include "MachineStack.hpp"
#include "OperandFactory.hpp"
#include "eOperandType.hpp"
#include <exception>
#include <iostream>
#include "InputLoop.hpp"
#include <tuple>
#include <vector>
#include <list>
#include <unordered_map>

auto _decompose_4(auto&&... args)
{
	auto&& s = std::get<MachineStack&>(args...);
	auto&& t = std::get<eOperandType>(args...);
	auto&& v = std::get<std::string>(args...);
	return std::make_tuple<MachineStack&, eOperandType, std::string>(s, t, v);
}

eOperandType getType(const std::string& t)
{
	using enum eOperandType;
	const std::unordered_map<std::string, eOperandType> type = {
		{ "int8",   Int8   },
		{ "int16",  Int16  },
		{ "int32",  Int32  },
		{ "float",  Float  },
		{ "double", Double },
	};
	if (!type.contains(t))
	{
		std::cerr << "!no " << t << '\n';
	}
	return type.at(t);
}

void _push(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	auto token = std::move(tokens.front());
	tokens.pop_front();
	auto operand_value = std::move(tokens.front());
	tokens.pop_front();
	stack.push(OperandFactory::createOperand(getType(token), operand_value));
}

void _assert(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	auto token = std::move(tokens.front());
	tokens.pop_front();
	auto operand_value = std::move(tokens.front());
	tokens.pop_front();
	stack.push(OperandFactory::createOperand(getType(token), operand_value));
}

void _pop(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.pop();
}

void _dump(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.dump();
}

void _add(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.add();
}

void _sub(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.sub();
}

void _mul(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.mul();
}

void _div(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.div();
}

void _mod(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.mod();
}

void _print(std::list<std::string>& tokens, MachineStack& stack)
{
	tokens.pop_front();
	stack.print();
}

void _exit(std::list<std::string>&, MachineStack&)
{}

void test1()
{
	static const std::unordered_map<std::string, decltype(&_pop)> instr = {
		{ "push",   _push },
		{ "assert", _assert },
		{ "pop",    _pop },
		{ "dump",   _dump },
		{ "add",    _add },
		{ "sub",    _sub },
		{ "mul",    _mul },
		{ "div",    _div },
		{ "mod",    _mod },
		{ "print",  _print },
		{ "exit",   _exit },
	};
	try {
		auto tokens = avm::inputLoop();
		MachineStack stack;
		while (!tokens.empty())
		{
			if (!instr.contains(tokens.front()))
			{
				std::cerr << "no " << tokens.front() << '\n';
			}
			instr.at(tokens.front())(tokens, stack);
		}
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
		exit(21);
	}
}

int main()
{
	test1();
}
