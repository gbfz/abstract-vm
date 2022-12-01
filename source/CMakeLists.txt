cmake_minimum_required(VERSION 3.2)

project(avm)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_options(-g -Wall -Wextra -Werror -std=c++20)
add_link_options(-g)

link_libraries(gmpxx gmp)

add_custom_target(BuildLiblinenoise ALL
	COMMAND make -C ${CMAKE_SOURCE_DIR}/linenoise
)
add_library(liblinenoise STATIC IMPORTED)
set_target_properties(liblinenoise PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/linenoise/liblinenoise.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/linenoise"
)
add_dependencies(liblinenoise BuildLiblinenoise)

add_executable(avm
	OperandFactory.cpp
	MachineStack.cpp
	Exceptions.cpp
	ArithmeticExceptions.cpp
	c_string.cpp
	InputLoop.cpp
	usage.cpp
	main.cpp
)
target_link_libraries(avm liblinenoise)
