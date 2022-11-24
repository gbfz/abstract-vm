#pragma once
#include <stdexcept>

namespace ft {

struct modulo_with_fractions_exception final : public std::exception
{ const char* what() const noexcept override; };

struct overflow_exception final : public std::exception
{ const char* what() const noexcept override; };

}
