#include "Exceptions.hpp"
#include "eOperandType.hpp"
#include <unordered_map>

namespace avm {

value_assert_exception::value_assert_exception(
		const std::string& expected,
		const std::string& actual) : std::exception()
{
	msg = "value mismatch in assert(). expected "
		+ expected + ", got " + actual + "";
}

const char* value_assert_exception::what() const noexcept
{ return msg.c_str(); }

type_assert_exception::type_assert_exception(eOperandType expected,
											 eOperandType actual)
	: std::exception()
{
	msg = "type mismatch in assert(). expected "
		+ toString(expected) + ", got " + toString(actual);
}

const char* type_assert_exception::what() const noexcept
{ return msg.c_str(); }

const char* div_by_zero_exception::what() const noexcept
{ return "division by zero attempted"; }

const char* mod_by_zero_exception::what() const noexcept
{ return "modulo by zero attempted"; }

const char* pop_exception::what() const noexcept
{ return "attempt to pop empty stack"; }

const char* pop_two_exception::what() const noexcept
{ return "attempt to pop two operands while stack size is less than 2"; }

const char* print_exception::what() const noexcept
{ return "attempt to print value with type other than int8"; }

const char* modulo_with_fractions_exception::what() const noexcept
{ return "modulo not supported for fractions"; }

const char* overflow_exception::what() const noexcept
{ return "last operation caused overflow"; }

}
