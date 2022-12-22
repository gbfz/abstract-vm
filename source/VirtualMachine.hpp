#pragma once
#include "Input.hpp"
#include "MachineStack.hpp"
#include <unordered_map>

using std::string_literals::operator""s;

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
	std::list<std::string> read_from_stdin();
	std::list<std::string> read_from_file(const char* filename = "");

public:
	int exec(const char* stream_name = "");

	VirtualMachine() = default;
	VirtualMachine(VirtualMachine&& other) = default;
	VirtualMachine& operator= (VirtualMachine&& other) = default;

	VirtualMachine(const VirtualMachine&) = delete;
	VirtualMachine& operator= (const VirtualMachine&) = delete;

	int usage() const;
};

}
