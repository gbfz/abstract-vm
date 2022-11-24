#include "ArithmeticExceptions.hpp"

namespace ft {

const char* modulo_with_fractions_exception::what() const noexcept
{ return "Modulo not supported for fractions"; }

const char* overflow_exception::what() const noexcept
{ return "Overflow!"; }

}
