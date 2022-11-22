#include "Operand.hpp"
#include <iostream>
#include <ostream>

std::ostream& operator<< (std::ostream& out, eOperandType type)
{
	using enum eOperandType;
	switch (type)
	{
		case Int8: return out << "Int8";
		case Int16: return out << "Int16";
		case Int32: return out << "Int32";
		case Float: return out << "Float";
		case Double: return out << "Double";
	}
	return out;
}

int main()
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
