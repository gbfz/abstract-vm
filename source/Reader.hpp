#pragma once
#include "linenoise/linenoise.h"
#include <algorithm>
#include <cerrno>
#include <fstream>
#include <string>
#include <list>

namespace avm {

enum class eInputSource
{
	file,
	tty,
};

template <eInputSource source>
struct Reader;

struct read_sentinel {};

template <eInputSource source>
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
		if constexpr(source == eInputSource::tty)
			return input == ";;";
		else return parent.stream.eof();
	}

	read_iterator& operator++ ()
	{
		if constexpr(source == eInputSource::file)
			if (parent.cur_line > parent.max_lines)
				throw std::runtime_error("File length is greater than chosen reader capacity");
		input = parent.getInput();
		return *this;
	}

	std::string operator* () const { return input; }
};

template <>
struct Reader<eInputSource::tty>
{
	std::string getInput()
	{
		std::string input;
		if (char* raw = linenoise("abstract-vm$ "))
		{
			input = raw;
			std::free(raw);
		} else if (errno)
			std::exit(2);
		return input;
	}

	read_iterator<eInputSource::tty> begin() { return {*this}; }
	read_sentinel end() { return {}; }
};

template <>
struct Reader<eInputSource::file>
{
	std::ifstream stream;
	size_t max_lines;
	size_t cur_line = 0;

	std::string getInput()
	{
		std::string buf;
		std::getline(stream, buf);
		++cur_line;
		return buf;
	}

	read_iterator<eInputSource::file> begin() { return { *this }; }
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
