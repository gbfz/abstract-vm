#include "VirtualMachine.hpp"
#include "Input.hpp"
#include <exception>

using std::string_literals::operator""s;

namespace avm {

VirtualMachine::VirtualMachine(VirtualMachine&& other)
	: tokens(std::move(other.tokens))
	, stack(std::move(other.stack)) {}

VirtualMachine& VirtualMachine::operator= (VirtualMachine&& other)
{
	tokens = std::move(other.tokens);
	stack = std::move(other.stack);
	return *this;
}

void VirtualMachine::exec_internal(const char* stream_name)
{
	if (stream_name == ""s)
		read_tokens_from_stdin();
	else
		read_tokens_from_file(stream_name);
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

void VirtualMachine::read_tokens_from_stdin()
{
	tokens = avm::readInput();
}

void VirtualMachine::read_tokens_from_file(const char* filename)
{
	std::ifstream file { filename };
	if (!file.is_open() || file.bad())
		throw std::invalid_argument("cannot open or read given file");
	tokens = avm::readInput(std::move(file));
}

}
