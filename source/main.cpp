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
	MachineStack ms;
	ms.push(eInstructionType::Push, eOperandType::Int8, "42");
	ms.push(eInstructionType::Push, eOperandType::Double, "0.21");
	ms.push(eInstructionType::Add);
	ms.dump();
}

int main()
{
	testMachine();
}
