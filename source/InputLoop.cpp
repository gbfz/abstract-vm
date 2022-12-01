#include "InputLoop.hpp"
#include <iostream>
#include <string>

namespace avm {

namespace {

	c_string getInput()
	{
		return std::move(linenoise("yo@ "));
	}

	auto isInputEndValid(const std::list<std::string>& tokens)
	{
		struct result_ok {
			bool ok = false;
			const char* msg;
			result_ok(bool b, const char* m = "") : ok(b), msg(m) {}
		};
		auto i = std::ranges::find(tokens, "exit");
		if (i == tokens.end())
			return result_ok { false, "No 'exit' instruction" };
		if (i != std::prev(tokens.end()))
			return result_ok { false, "Excess instructions after 'exit'" };
		return result_ok { true };
	}

}

std::list<std::string> inputLoop()
{
	std::list<std::string> tokens;
	for (auto&& input = getInput(); input != ";;"; input = getInput())
	{
		auto begin = input.begin();
		bool parse_ok = avm::parser::parse_string(begin, input.end(), tokens);
		if (parse_ok)
			linenoiseHistoryAdd(*input);
		else std::cout << "Error at: " << std::string(begin, input.end()) << '\n';
	}
	auto [input_ok, msg] = isInputEndValid(tokens);
	if (!input_ok)
		throw std::runtime_error(msg);
	return tokens;
}

}
