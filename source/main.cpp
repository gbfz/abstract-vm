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
	using enum eInstructionType;
	using enum eOperandType;
	MachineStack stack_a;
	stack_a.push(Push, Int8, "42");
	stack_a.push(Assert, Int8, "42");
	stack_a.push(Push, Double, "0.21");
	stack_a.push(Assert, Double, "0.21");
	stack_a.push(Add);
	stack_a.push(Assert, Double, "42.21");
	stack_a.push(Push, Int8, "97");
	stack_a.push(Assert, eOperandType::Int8, "97");
	stack_a.push(Print);
	stack_a.dump();
	std::cout << "\n---\n";
	stack_a.exec();
	// MachineStack stack_b;
	// stack_b.fill(std::move(stack_a.is), std::move(stack_a.vs));
	// stack_b.dump();
	// std::cout << "\n---\n";
	// stack_b.exec();
	// for (auto&& o : stack_b.vs) { std::cout << o->getType() << ", " << o->toString() << '\n'; }
}

int main()
{
	testMachine();
}
