#include "Exceptions.hpp"
#include <unordered_map>

namespace avm {

value_assert_exception::value_assert_exception(
		const std::string& expected,
		const std::string& actual) : std::exception()
{
	msg = "Value mismatch in assert(). Expected "
		+ expected + ", got " + actual + "";
}

const char* value_assert_exception::what() const noexcept
{ return msg.c_str(); }

type_assert_exception::type_assert_exception(
		const eOperandType expected,
		const eOperandType actual) : std::exception()
{
	const std::unordered_map<eOperandType, std::string> tm = {
		{ eOperandType::Int8,   "int8"   },
		{ eOperandType::Int16,  "int16"  },
		{ eOperandType::Int32,  "int32"  },
		{ eOperandType::Float,  "float"  },
		{ eOperandType::Double, "double" },
	};
	msg = "Type mismatch in assert(). Expected "
		+ tm.at(expected) + ", got " + tm.at(actual);
}

const char* type_assert_exception::what() const noexcept
{ return msg.c_str(); }

const char* div_by_zero_exception::what() const noexcept
{ return "Division by zero attempted"; }

const char* mod_by_zero_exception::what() const noexcept
{ return "Modulo by zero attempted"; }

const char* pop_exception::what() const noexcept
{ return "Attempt to pop empty stack"; }

const char* pop_two_exception::what() const noexcept
{ return "Attempt to pop two operands while stack size is less than 2"; }

const char* print_exception::what() const noexcept
{ return "Attempt to print value with type other than int8"; }

const char* modulo_with_fractions_exception::what() const noexcept
{ return "Modulo not supported for fractions"; }

const char* overflow_exception::what() const noexcept
{ return "Overflow!"; }

}
