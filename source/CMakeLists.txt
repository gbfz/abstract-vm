cmake_minimum_required(VERSION 3.2)

project(avm)

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_options(-g -Wall -Wextra -Werror -std=c++20)

link_libraries(gmpxx gmp)

add_executable(avm
	OperandFactory.cpp
	InstructionStack.cpp
	ValueStack.cpp
	MachineStack.cpp
	main.cpp
)
