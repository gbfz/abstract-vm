#include "Adders.hpp"
#include <iostream>

void test(const std::string& lhs, const std::string& rhs, auto f, const std::string& exp)
{
	auto r = f(lhs, rhs);
	if (r != exp)
	{
		std::cout << lhs << ", " << rhs << " != " << exp << '\n';
		std::cout << "Actual: " << r << '\n';
		exit(1);
	} else std::cout << lhs << ", " << rhs << " oк\n";
}

#include <regex>

int main()
{
	/*
	test("0", "500", mulIntegral, "0");
	test("1", "1", mulIntegral, "1");
	test("2", "2", mulIntegral, "4");
	test("10", "3", mulIntegral, "30");
	test("42", "10", mulIntegral, "420");
	test("1000", "103", mulIntegral, "103000");
	test("103", "1000", mulIntegral, "103000");
	test("117", "420", mulIntegral, "49140");
	test("420", "117", mulIntegral, "49140");

	test("0", "0", addIntegral, "0");
	test("00", "0", addIntegral, "0");
	test("9", "1", addIntegral, "10");
	test("19", "1", addIntegral, "20");
	test("113", "77", addIntegral, "190");
	test("17", "442", addIntegral, "459");
	test("2147483647", "8000000000", addIntegral, "10147483647");

	test("13", "55", addFloating, "68.0");
	test("5", "0.052", addFloating, "5.052");
	test("0", "1.14", addFloating, "1.14");
	test("0.0", "0.0", addFloating, "0.0");
	test("0.3", "1.3", addFloating, "1.6");
	test("0.3", "1.7", addFloating, "2.0");
	test("8.2", "1.8", addFloating, "10.0");
	test("0.003", "1.0", addFloating, "1.003");
	test("1.0", "0.003", addFloating, "1.003");
	test("1.17", "3.442", addFloating, "4.612");
	test("1.0000000000001", "0.0000000000000000000001", addFloating, "1.0000000000001000000001");
	*/

	test("1", "1", mulFloating, "1.0");
	test("0.1", "10", mulFloating, "1.0");
	test("0.25", "10", mulFloating, "2.5");
	test("100", "3.3", mulFloating, "330.0");
	test("515.44", ".0000003", mulFloating, "0.000154632");
	test("0.1", "0.00000000000002", mulFloating, "0.000000000000002");
}
