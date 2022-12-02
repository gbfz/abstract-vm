#include "InputLoop.hpp"
#include "linenoise/linenoise.h"
#include "Reader.hpp"
#include "InputHandler.hpp"
#include "c_string.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

namespace avm {

namespace {

	template <inputSource source>
	std::list<std::string> inputLoop(Reader<source>&& reader)
	{
		std::list<std::string> tokens;
		inputHandler<source> ihdl;
		ihdl.setCompletion();
		for (auto&& input : reader)
		{
			auto begin = input.begin();
			bool parse_ok = avm::parser::parse_string(begin, input.end(), tokens);
			if (!parse_ok)
				std::cout << "Error at: " << std::string(begin, input.end()) << '\n'; // TODO 
			ihdl.addHistory(input.data());
		}
		auto [input_ok, msg] = reader.isInputEndValid(tokens);
		if (!input_ok)
			throw std::runtime_error(msg);
		return tokens;
	}

}

std::list<std::string> readInput()
{
	return inputLoop(Reader<inputSource::tty>());
}

std::list<std::string> readInput(std::ifstream file)
{
	auto reader = Reader<inputSource::file>(std::move(file));
	return inputLoop(std::move(reader));
}

}
