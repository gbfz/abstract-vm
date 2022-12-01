#pragma once
#include "2Parse.hpp"
#include "linenoise/linenoise.h"
#include "c_string.hpp"
#include <algorithm>

namespace avm {

std::list<std::string> readInput();
std::list<std::string> readInput(std::ifstream file);

}
