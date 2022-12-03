cmake_minimum_required(VERSION 3.24)

project(avm)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_options(-g -Wall -Wextra -Werror -std=c++20)
add_link_options(-g)

link_libraries(gmpxx gmp)

add_library(liblinenoise STATIC IMPORTED)
add_custom_target(buildliblinenoise ALL
	COMMAND make lib
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/linenoise)
set_target_properties(liblinenoise PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/linenoise/liblinenoise.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/linenoise")
add_dependencies(liblinenoise buildliblinenoise)

add_executable(avm
	OperandFactory.cpp
	MachineStack.cpp
	Exceptions.cpp
	InputLoop.cpp
	usage.cpp
	main.cpp)
target_link_libraries(avm liblinenoise)
