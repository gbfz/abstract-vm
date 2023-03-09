cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(avm)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g2 -Wall -Wextra -Werror -Wpedantic)
add_link_options(-g2)

add_library(linenoise STATIC
	linenoise/linenoise.c
	linenoise/linenoise.h
)
target_compile_definitions(linenoise PRIVATE LINENOISE_PRIVATE)

add_executable(avm
	Input.cpp
	InputCompletion.cpp
	OperandFactory.cpp
	MachineStack.cpp
	eOperandType.cpp
	VirtualMachine.cpp
	main.cpp
)
target_link_libraries(avm PRIVATE
	gmpxx
	gmp
	linenoise
)
target_include_directories(avm PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
