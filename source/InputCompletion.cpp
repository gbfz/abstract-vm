#include "InputCompletion.hpp"

namespace avm {

namespace {

	const std::array<const char*, 5>& getDict(const std::string& type)
	{
		static std::array<std::array<const char*, 5>, 2> dict = {{
			{ "push int8(", "push int16(", "push int32(", "push float(", "push double(" },
			{ "assert int8(", "assert int16(", "assert int32(", "assert float(", "assert double(" }
		}};
		return dict[type == "assert"];
	}

	auto setOperandCompletion(const std::string& buf, const auto dict, linenoiseCompletions* lc)
	{
		if (buf.find('i') != std::string::npos)
		{
			linenoiseAddCompletion(lc, dict[0]);
			linenoiseAddCompletion(lc, dict[1]);
			return linenoiseAddCompletion(lc, dict[2]);
		}
		if (buf.find('f') != std::string::npos)
			return linenoiseAddCompletion(lc, dict[3]);
		if (buf.find('d') != std::string::npos)
			return linenoiseAddCompletion(lc, dict[4]);
	}

	auto setFirstLetterCompletion(const std::string& buf, linenoiseCompletions* lc)
	{
		switch (buf.front())
		{
			case 'p': linenoiseAddCompletion(lc, "push");
					  linenoiseAddCompletion(lc, "pop");
					  return linenoiseAddCompletion(lc, "print");
			case 'a': linenoiseAddCompletion(lc, "assert");
					  return linenoiseAddCompletion(lc, "add");
			case 'd': linenoiseAddCompletion(lc, "dump");
					  return linenoiseAddCompletion(lc, "div");
			case 'm': linenoiseAddCompletion(lc, "mul");
					  return linenoiseAddCompletion(lc, "mod");
			case 's': return linenoiseAddCompletion(lc, "sub");
			case 'e': return linenoiseAddCompletion(lc, "exit");
		}
	}


}

void completion(const char* b, linenoiseCompletions* lc)
{
	std::string buf = b;
	if (buf.starts_with("push"))
		return setOperandCompletion(buf, getDict("push"), lc);
	if (buf.starts_with("assert"))
		return setOperandCompletion(buf, getDict("assert"), lc);
	setFirstLetterCompletion(buf, lc);
}

}
