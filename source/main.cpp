#include "VirtualMachine.hpp"

int usage();

int main(int ac, char** av)
{
	avm::VirtualMachine vm;

	switch (ac)
	{
		case 1: if (not isatty(fileno(stdin))) // read from pipe
					return vm.exec("/dev/stdin");
				return vm.exec();
		case 2: if (av[1] == "-h"s || av[1] == "--help"s)
					return vm.usage();
				return vm.exec(av[1]);
	}
	return vm.usage();
}
