#pragma once
#include "Input.hpp"
#include "MachineStack.hpp"
#include "Exceptions.hpp"
#include <typeindex>
#include <unordered_map>

namespace avm {

class VirtualMachine
{
private:
	std::list<std::string> tokens;
	MachineStack stack;
	std::unordered_map<std::string, std::function<void()>> handlers =
	{
		{ "push",   [&] { stack.push(tokens); } },
		{ "assert", [&] { stack.assert(tokens); } },
		{ "save",   [&] { stack.save(tokens); } },
		{ "load",   [&] { stack.load(tokens); } },
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

	void exec_internal(const char* stream_name);
	void read_tokens_from_stdin();
	void read_tokens_from_file(const char* filename = "");

public:
	int exec(const char* stream_name = "");

	std::list<std::string> get_token_stream();
	std::list<std::string> get_token_stream(const char* filename);

	VirtualMachine() = default;
	VirtualMachine(VirtualMachine&& other);
	VirtualMachine& operator= (VirtualMachine&& other);

	VirtualMachine(const VirtualMachine&) = delete;
	VirtualMachine& operator= (const VirtualMachine&) = delete;
};

}
