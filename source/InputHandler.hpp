#pragma once
#include "Parser.hpp"
#include "linenoise/linenoise.h"
#include "Reader.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <array>

using std::literals::operator""s;

namespace avm {

std::list<std::string> readInput();
std::list<std::string> readInput(std::ifstream file);

template <inputSource source>
class inputHandler;

template <>
class inputHandler<inputSource::tty>
{
private:
	static const std::array<const char*, 5> getDict(const std::string& type)
	{
		static std::array<std::array<const char*, 5>, 2> dict = {{
			{ "push int8(", "push int16(", "push int32(", "push float(", "push double(" },
			{ "assert int8(", "assert int16(", "assert int32(", "assert float(", "assert double(" }
		}};
		return dict[type == "assert"];
	}

	static auto setOperandCompletion(const std::string& buf, const auto dict, linenoiseCompletions* lc)
	{
		if (buf.find('i') != std::string::npos)
		{
			linenoiseAddCompletion(lc, dict[0]);
			linenoiseAddCompletion(lc, dict[1]);
			return linenoiseAddCompletion(lc, dict[2]);
		}
		if (buf.find('f') != std::string::npos)
			return linenoiseAddCompletion(lc, dict[3]);
		if (buf.find('d') != std::string::npos)
			return linenoiseAddCompletion(lc, dict[4]);
	}

	static auto setFirstLetterCompletion(const std::string& buf, linenoiseCompletions* lc)
	{
		switch (buf.front())
		{
			case 'p': linenoiseAddCompletion(lc, "push");
					  linenoiseAddCompletion(lc, "pop");
					  return linenoiseAddCompletion(lc, "print");
			case 'a': linenoiseAddCompletion(lc, "assert");
					  return linenoiseAddCompletion(lc, "add");
			case 'd': linenoiseAddCompletion(lc, "dump");
					  return linenoiseAddCompletion(lc, "div");
			case 'm': linenoiseAddCompletion(lc, "mul");
					  return linenoiseAddCompletion(lc, "mod");
			case 's': return linenoiseAddCompletion(lc, "sub");
			case 'e': return linenoiseAddCompletion(lc, "exit");
		}
	}

	static auto completion(const char* b, linenoiseCompletions* lc)
	{
		std::string buf = b;
		if (buf.starts_with("push"))
			return setOperandCompletion(buf, getDict("push"), lc);
		if (buf.starts_with("assert"))
			return setOperandCompletion(buf, getDict("assert"), lc);
		setFirstLetterCompletion(buf, lc);
	}

public:
	Reader<inputSource::tty> reader;

	void handle_error(const std::string& s)
	{ std::cerr << "invalid input : " << s << '\n'; }

	auto addHistory(const char* line) { linenoiseHistoryAdd(line); }
	inputHandler() { linenoiseSetCompletionCallback(completion); }
};

template <>
class inputHandler<inputSource::file>
{
public:
	Reader<inputSource::file> reader;

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
	void handle_error(const std::string& s)
	{
		auto msg = "invalid input on line "s + std::to_string(reader.cur_line)
				 + ": '" + s + "'\n"
				 + "execution terminated";
		throw std::runtime_error(msg);
	}

	auto addHistory(const char*) {}

	inputHandler(std::ifstream stream) : reader(std::move(stream)) {}
	inputHandler(inputHandler&& other) = default;
	inputHandler& operator= (inputHandler&& other) = default;
	~inputHandler() = default;
};

}
