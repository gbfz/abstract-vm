#pragma once
#include "linenoise/linenoise.h"
#include <string>
#include <array>

namespace avm {

void completion(const char* buf, linenoiseCompletions* lc);

}
