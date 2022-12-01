#include "InputLoop.hpp"
#include "Reader.hpp"
#include <fstream>
#include <string>

namespace avm {

namespace {

	std::list<std::string> inputLoop(auto&& reader)
	{
		std::list<std::string> tokens;
		for (auto&& input : reader)
		{
			auto begin = input.begin();
			bool parse_ok = avm::parser::parse_string(begin, input.end(), tokens);
			if (parse_ok)
				linenoiseHistoryAdd(input.data());
			else std::cout << "Error at: " << std::string(begin, input.end()) << '\n';
		}
		auto [input_ok, msg] = reader.isInputEndValid(tokens);
		if (!input_ok)
			throw std::runtime_error(msg);
		return tokens;
	}

}

std::list<std::string> readInput()
{ return inputLoop(Reader<inputSource::tty>()); }

std::list<std::string> readInput(std::ifstream file)
{ return inputLoop(Reader<inputSource::file>(std::move(file))); }

}
