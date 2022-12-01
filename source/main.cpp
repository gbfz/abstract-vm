#include "MachineStack.hpp"
#include "OperandFactory.hpp"
#include "eOperandType.hpp"
#include "InputLoop.hpp"
#include <unordered_map>
#include <exception>
#include <list>
#include <iostream>

eOperandType toEnum(const std::string& t)
{
	using enum eOperandType;
	static const std::unordered_map<std::string, eOperandType> type = {
		{ "int8",   Int8   },
		{ "int16",  Int16  },
		{ "int32",  Int32  },
		{ "float",  Float  },
		{ "double", Double },
	};
	return type.at(t);
}

void _push(std::list<std::string>& tokens, MachineStack& stack)
{
	auto type = std::move(tokens.front());
	tokens.pop_front();
	auto operand_value = std::move(tokens.front());
	tokens.pop_front();
	stack.push(OperandFactory::createOperand(toEnum(type), operand_value));
}

void _assert(std::list<std::string>& tokens, MachineStack& stack)
{
	auto token = std::move(tokens.front());
	tokens.pop_front();
	auto operand_value = std::move(tokens.front());
	tokens.pop_front();
	stack.assert(toEnum(token), operand_value);
}

void _pop(auto&, MachineStack& stack) { stack.pop(); }
void _add(auto&, MachineStack& stack) { stack.add(); }
void _sub(auto&, MachineStack& stack) { stack.sub(); }
void _mul(auto&, MachineStack& stack) { stack.mul(); }
void _div(auto&, MachineStack& stack) { stack.div(); }
void _mod(auto&, MachineStack& stack) { stack.mod(); }
void _dump(auto&, MachineStack& stack) { stack.dump(); }
void _print(auto&, MachineStack& stack) { stack.print(); }
void _exit(auto&, MachineStack&) {}

void exec(const auto& handlers)
{
	MachineStack stack;
	auto tokens = avm::inputLoop();
	while (!tokens.empty())
	{
		auto instruction = std::move(tokens.front());
		auto handler = handlers.at(instruction);
		tokens.pop_front();
		handler(tokens, stack);
	}
}

void test1()
{
	// const auto getTwo = [&]()
	// {
	// 	auto type = toEnum(std::move(tokens.front()));
	// 	tokens.pop_front();
	// 	auto value = std::move(tokens.front());
	// 	tokens.pop_front();
	// 	return std::make_pair(type, value);
	// };

	static const std::unordered_map<std::string, decltype(&_push)> handlers = {
		{ "push", _push }, { "assert", _assert },
		{ "pop" , _pop  }, { "dump"  , _dump   },
		{ "add" , _add  }, { "sub"   , _sub    },
		{ "mul" , _mul  }, { "div"   , _div    },
		{ "mod" , _mod  }, { "print" , _print  },
		{ "exit", _exit }
	};
	try {
		exec(handlers);
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
		exit(21);
	}
}

int main()
{
	test1();
}
