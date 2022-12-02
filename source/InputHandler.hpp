#pragma once
#include "linenoise/linenoise.h"
#include "Reader.hpp"
#include <string>
#include <vector>
#include <array>

using std::literals::operator""s;

namespace avm {

template <inputSource source>
struct inputHandler;

template <>
struct inputHandler<inputSource::tty>
{
private:
	static const char** getDict(const char* pref)
	{
		static const char* dict[] = {
			"push int8(", "push int16(", "push int32(", "push float(", "push double(",
			"assert int8(", "assert int16(", "assert int32(", "assert float(", "assert double(",
		};
		if (pref == "push"s)
			return dict;
		return dict + 5;
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
	auto addHistory(const char* line) { linenoiseHistoryAdd(line); }
	auto setCompletion() { linenoiseSetCompletionCallback(completion); }
};

template <>
struct inputHandler<inputSource::file>
{
	auto addHistory(const char*) {}
	auto setCompletion() {}
};

}
