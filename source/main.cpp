#include "IOperand.hpp"
#include "Operand.hpp"
#include "MachineStack.hpp"
#include "Parse.hpp"
#include <iostream>
#include <ostream>

void testOperand()
{
	using enum eOperandType;
	auto a = OperandFactory::createOperand(Int32, "120");
	auto b = OperandFactory::createOperand(Double, "10132");
	auto d = OperandFactory::createOperand(Float, "12.1033");
	auto c = *d - *b;
	std::cout << c->toString() << '\n';
	std::cout << c->getType() << '\n';
	std::cout << c->getPrecision() << '\n';
}

void testMachine()
{
	using enum eOperandType;
	MachineStack sa;
	sa.push(Int8, "42");
	sa.assert(Int8, "42");
	sa.push(Double, "0.211111");
	sa.assert(Double, "0.211111");
	sa.add();
	sa.assert(Double, "42.211111");
	sa.pop();
	sa.push(Float, "0.211111");
	sa.push(Double, "42.211111");
	sa.sub();
	sa.assert(Double, "42.0");
	sa.push(Int8, "0");
	sa.push(Int32, "500");
	sa.mul();
	sa.assert(Int32, "0");
	sa.dump();
}

void test2()
{
	using enum eOperandType;
	MachineStack sa;
	sa.push(Int8, "42");
	sa.push(Double, "0.211111");
	sa.pop();
	sa.pop();
	sa.push(Int32, "227");
	// sa.print();
	// sa.dump();
	try {
		sa.assert(Int16, "227");
	} catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void test3()
{
	using enum eOperandType;
	MachineStack sa;
	sa.push(Float, "10.123456987654323456789");
	sa.push(Float, "387324234.123456789123456789123456789123456789");
	try {
		sa.mod();
	} catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void testParse()
{
	// std::string i = "push int8(0)\ndump\nexit";
	std::string i = "push int8(0)\nexit\n";
	MachineStack s;
	auto begin = i.begin();
	bool b = ft::parser::parse(begin, i.end(), s);
	std::cout << std::boolalpha << b << '\n';
	if (begin != i.end())
	{
		std::cout << '\n';
		std::cout << std::string(begin, i.end()) << '\n';
	}
}

int main()
{
	// testMachine();
	// test2();
	// test3();
	testParse();
}
