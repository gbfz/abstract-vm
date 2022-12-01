#pragma once
#include "linenoise/linenoise.h"
#include "c_string.hpp"
#include <algorithm>
#include <fstream>
#include <string>
#include <list>

namespace avm {

enum class inputSource {
	file,
	tty,
};

template <inputSource source>
struct Reader;

struct read_sentinel {};

template <inputSource source>
class read_iterator
{
private:
	Reader<source>& parent;
	using strT = std::conditional_t<source == inputSource::tty, c_string, std::string>;
	strT input;
	size_t cur_line = 0;
public:
	read_iterator(Reader<source>& p) : parent(p), input(p.getInput()) {}
	read_iterator(const read_iterator&) = delete;
	read_iterator& operator= (const read_iterator&) = delete;
	read_iterator(read_iterator&&) = delete;
	read_iterator& operator= (read_iterator&&) = delete;

	bool operator== (read_sentinel) const
	{
		if constexpr(source == inputSource::tty)
			return input == ";;";
		else return input == "exit";
	}

	read_iterator& operator++ ()
	{
		if constexpr(source == inputSource::file)
			if (++cur_line > parent.max_lines)
				throw std::runtime_error("File length is greater than chosen reader capacity");
		input = parent.getInput();
		return *this;
	}

	strT operator* () const { return input; }
};

template <>
struct Reader<inputSource::tty>
{
	c_string getInput() { return std::move(linenoise("yo@ ")); }
	auto isInputEndValid(const std::list<std::string>& tokens)
	{
		auto i = std::ranges::find(tokens, "exit");
		if (i == tokens.end())
			return std::make_pair(false, "No 'exit' instruction");
		return std::make_pair(true, "");
	}

	read_iterator<inputSource::tty> begin() { return {*this}; }
	read_sentinel end() { return {}; }
};

template <>
struct Reader<inputSource::file>
{
	std::ifstream stream;
	size_t max_lines;

	std::string getInput()
	{
		std::string buf;
		std::getline(stream, buf);
		return buf;
	}

	auto isInputEndValid(const std::list<std::string>&) { return std::make_pair(true, ""); }

	read_iterator<inputSource::file> begin() { return {*this}; }
	read_sentinel end() { return {}; }

	Reader() = default;
	~Reader() = default;
	Reader(std::ifstream fl, size_t max_lines = 100) : stream(std::move(fl)), max_lines(max_lines) {}
	Reader(Reader&& other) : stream(std::move(other.stream)) {}
	Reader& operator= (Reader&& other)
	{
		stream = std::move(other.stream);
		max_lines = other.max_lines;
		return *this;
	}
};

}
