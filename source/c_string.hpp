#pragma once
#include <ostream>

class c_string
{
private:
	char* _ptr = nullptr;
	size_t _size = 0;
public:
	c_string() = default;
	c_string(char* ptr) noexcept;
	c_string(const char* ptr);

	c_string(const c_string& other);
	c_string& operator= (const c_string& other);

	c_string(c_string&& other) noexcept;
	c_string& operator= (c_string&& other) noexcept;

	~c_string() noexcept;

	size_t size() const noexcept;
	const char* data() const noexcept;

	char* begin() noexcept;
	const char* begin() const noexcept;
	char* end() noexcept;
	const char* end() const noexcept;

	std::strong_ordering operator<=>(const c_string& other) const;
	std::strong_ordering operator<=>(const char* other) const;
	bool operator== (const char* other) const;

	std::ostream& operator<< (std::ostream& out);
};
