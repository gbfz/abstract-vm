#pragma once
#include "Parser.hpp"
#include "Reader.hpp"
#include "InputCompletion.hpp"
#include "linenoise/linenoise.h"
#include <ios>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace avm {

std::list<std::string> readInput();
std::list<std::string> readInput(std::ifstream file);

template <eInputSource source>
struct InputHandler;

template <>
struct InputHandler<eInputSource::tty>
{
	Reader<eInputSource::tty> reader;

	void handle_error(const std::string& s, std::ostream& out = std::cerr)
	{ out << "invalid input : " << s << '\n'; }

	auto addHistory(const char* line) { linenoiseHistoryAdd(line); }

	InputHandler() { linenoiseSetCompletionCallback(avm::completion); }
	InputHandler(InputHandler&& other) = default;
	InputHandler& operator= (InputHandler&& other) = default;
	InputHandler(const InputHandler& other) = delete;
	InputHandler& operator= (const InputHandler& other) = delete;
	~InputHandler() = default;
};

template <>
struct InputHandler<eInputSource::file>
{
	Reader<eInputSource::file> reader;
	bool had_error = false;

	void handle_error(const std::string& s, std::ostream& out = std::cerr)
	{
		out << "invalid input on line " + std::to_string(reader.cur_line) + ": '" + s + "'";
		if (!had_error && (had_error = true))
			 out << ". NOTE: i'll parse the rest of the file but won't execute it.";
		out << '\n';
		for (auto&& input : reader)
		{
			auto begin = input.begin();
			auto parse_ok = avm::parser::parse_discard(begin, input.end());
			if (!parse_ok)
				handle_error({begin, input.end()}, out);
		}
		throw std::invalid_argument("invalid input in given file");
	}

	auto addHistory(const char*) {}

	InputHandler(std::ifstream stream) : reader(std::move(stream)) {}
	InputHandler(InputHandler&& other) = default;
	InputHandler& operator= (InputHandler&& other) = default;
	InputHandler(const InputHandler& other) = delete;
	InputHandler& operator= (const InputHandler& other) = delete;
	~InputHandler() = default;
};

}
