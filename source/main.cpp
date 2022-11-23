#include "IOperand.hpp"
#include "Operand.hpp"
#include "MachineStack.hpp"
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
	sa.push(Int8, "227");
	sa.print();
	// sa.dump(std::cout);
}

int main()
{
	// testMachine();
	test2();
}
