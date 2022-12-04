#include "Input.hpp"

namespace avm {

namespace {

	std::list<std::string> inputLoop(auto&& handle)
	{
		std::list<std::string> tokens;
		for (auto&& input : handle.reader)
		{
			auto begin = input.begin();
			auto parse_ok = avm::parser::parse_string(begin, input.end(), tokens);
			if (!parse_ok)
				handle.handle_error({begin, input.end()});
			handle.addHistory(input.data());
		}
		if (std::ranges::find(tokens, "exit") == tokens.end())
			throw std::runtime_error("No 'exit' instruction\n");
		return tokens;
	}

}

std::list<std::string> readInput()
{
	auto input_handle = InputHandler<eInputSource::tty>();
	return inputLoop(input_handle);
}

std::list<std::string> readInput(std::ifstream file)
{
	auto input_handle = InputHandler<eInputSource::file>(std::move(file));
	return inputLoop(input_handle);
}

}
