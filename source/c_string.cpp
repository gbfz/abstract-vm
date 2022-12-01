#include "c_string.hpp"
#include <cstring>
#include <stdexcept>
#include <utility>

c_string::c_string(char* ptr) noexcept
	: _ptr(std::exchange(ptr, nullptr))
	, _size(_ptr ? strlen(_ptr) : 0)
{}

c_string::c_string(const char* ptr)
	: _ptr(strdup(ptr))
	, _size(_ptr ? strlen(_ptr) : 0)
{ if (!_ptr) throw std::bad_alloc(); }

c_string::c_string(const c_string& other)
	: _ptr(strdup(other._ptr))
	, _size(other._size)
{ if (!_ptr) throw std::bad_alloc(); }

c_string& c_string::operator= (const c_string& other)
{
	char* t = strdup(other._ptr);
	if (!t) throw std::bad_alloc();
	::free(_ptr);
	_ptr = t;
	_size = other._size;
	return *this;
}

c_string::c_string(c_string&& other) noexcept
	: _ptr(std::exchange(other._ptr, nullptr))
	, _size(other._size)
{}

c_string& c_string::operator= (c_string&& other) noexcept
{
	::free(_ptr);
	_ptr = std::exchange(other._ptr, nullptr);
	_size = std::exchange(other._size, 0);
	return *this;
}

c_string::~c_string() noexcept { ::free(_ptr); }

size_t c_string::size() const noexcept { return _size; }
const char* c_string::get() const noexcept { return _ptr; }
const char* c_string::operator* () const noexcept { return _ptr; }

char* c_string::begin() noexcept { return _ptr; }
const char* c_string::begin() const noexcept { return _ptr; }
char* c_string::end() noexcept { return _ptr + _size; }
const char* c_string::end() const noexcept { return _ptr + _size; }

std::strong_ordering c_string::operator<=>(const c_string& other) const
{
	if (!other._ptr)
		throw std::runtime_error("Comparing c_string with nullptr");
	int rel = strncmp(_ptr, other._ptr, std::min(_size, other._size));
	return rel <=> 0;
}

std::strong_ordering c_string::operator<=>(const char* other) const
{
	if (!other)
		throw std::runtime_error("Comparing c_string with nullptr");
	int rel = strncmp(_ptr, other, std::min(_size, strlen(other)));
	return rel <=> 0;
}

bool c_string::operator== (const char* other) const
{ return (*this <=> other) == 0; }

std::ostream& c_string::operator<< (std::ostream& out)
{
	out << _ptr;
	return out;
}
