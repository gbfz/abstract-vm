cmake_minimum_required(VERSION 3.24)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(avm)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -Wall -Wextra -Werror -Wno-cast-function-type)

add_library(linenoise STATIC
	linenoise/linenoise.c
)

link_libraries(gmpxx gmp linenoise)

add_executable(avm
	Input.cpp
	InputCompletion.cpp
	OperandFactory.cpp
	MachineStack.cpp
	Exceptions.cpp
	eOperandType.cpp
	VirtualMachine.cpp
	usage.cpp
	main.cpp
)
