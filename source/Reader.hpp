#pragma once
#include "linenoise/linenoise.h"
#include <algorithm>
#include <cerrno>
#include <fstream>
#include <string>
#include <list>

namespace avm {

enum class inputSource
{
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
	std::string input;

public:
	read_iterator(Reader<source>& p) : parent(p), input(p.getInput()) {}
	read_iterator(const read_iterator&) = delete;
	read_iterator& operator= (const read_iterator&) = delete;
	read_iterator(read_iterator&&) = delete;
	read_iterator& operator= (read_iterator&&) = delete;

	bool operator== (read_sentinel) const
	{
		if constexpr(source == inputSource::tty)
		{
			if (errno == EAGAIN)
				std::exit(1);
			return input == ";;";
		}
			// return input == ";;";
		else return parent.stream.eof();
	}

	read_iterator& operator++ ()
	{
		if constexpr(source == inputSource::file)
			if (++parent.cur_line > parent.max_lines)
				throw std::runtime_error("File length is greater than chosen reader capacity");
		input = parent.getInput();
		return *this;
	}

	std::string operator* () const { return input; }
};

template <>
struct Reader<inputSource::tty>
{
	std::string getInput()
	{
		std::string input;
		if (char* raw = linenoise("abstract-vm$ "))
		{
			input = raw;
			std::free(raw);
		}
		return input;
	}

	read_iterator<inputSource::tty> begin() { return {*this}; }
	read_sentinel end() { return {}; }
};

template <>
struct Reader<inputSource::file>
{
	std::ifstream stream;
	size_t max_lines;
	size_t cur_line = 1;

	std::string getInput()
	{
		std::string buf;
		std::getline(stream, buf);
		return buf;
	}

	read_iterator<inputSource::file> begin() { return {*this}; }
	read_sentinel end() { return {}; }

	Reader() = default;
	~Reader() = default;
	Reader(std::ifstream fl, size_t max_lines = 1024) : stream(std::move(fl)), max_lines(max_lines) {}
	Reader(Reader&& other) : stream(std::move(other.stream)) {}
	Reader& operator= (Reader&& other)
	{
		stream = std::move(other.stream);
		max_lines = other.max_lines;
		cur_line = other.cur_line;
		return *this;
	}
};

}
