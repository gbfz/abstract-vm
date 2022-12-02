#pragma once
#include "eOperandType.hpp"
#include <stdexcept>
#include <string>

namespace avm {

struct value_assert_exception final : public std::exception
{
	value_assert_exception(const std::string& expected,
						   const std::string& actual);
	const char* what() const noexcept override;
private:
	std::string msg;
};

struct type_assert_exception final : public std::exception
{
	type_assert_exception(eOperandType expected,
						  eOperandType actual);
	const char* what() const noexcept override;
private:
	std::string msg;
};

struct div_by_zero_exception final : public std::exception
{ const char* what() const noexcept override; };

struct mod_by_zero_exception final : public std::exception
{ const char* what() const noexcept override; };

struct pop_exception final : public std::exception
{ const char* what() const noexcept override; };

struct pop_two_exception final : public std::exception
{ const char* what() const noexcept override; };

struct print_exception final : public std::exception
{ const char* what() const noexcept override; };

struct modulo_with_fractions_exception final : public std::exception
{ const char* what() const noexcept override; };

struct overflow_exception final : public std::exception
{ const char* what() const noexcept override; };

}
