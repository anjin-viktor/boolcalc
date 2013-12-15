#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/dynamic_bitset.hpp>
#include "BoolCalc.h"

using namespace bcc;

BOOST_AUTO_TEST_SUITE(serialize);


BOOST_AUTO_TEST_CASE(expr_1)
{
	std::vector<bool> serializeData = Function::serialize("(x0 | x1) & (x2 + x3) & !(x4 | x5)");
	Function f(serializeData);


	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000101"))), 1);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("000111"))), 1);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001001"))), 1);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001010"))), 1);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001011"))), 1);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001110"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("001111"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010110"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("010111"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011110"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("011111"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("100111"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101110"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("101111"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110110"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("110111"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111000"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111001"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111010"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111011"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111100"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111101"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111110"))), 0);
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("111111"))), 0);
}


BOOST_AUTO_TEST_CASE(expr_2)
{
	std::vector<bool> serializeData = Function::serialize("(x0 | x1) & (x1 + x2) | x3 + x4 + 1");
	Function expr(serializeData);


	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00000"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00001"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00010"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00011"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00100"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00101"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("00111"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01000"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01001"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01010"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01011"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01100"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01101"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01110"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("01111"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10000"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10001"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10010"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10011"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10100"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10101"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10110"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("10111"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11000"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11001"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11010"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11011"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11100"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11101"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11110"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("11111"))), 1);
}


BOOST_AUTO_TEST_CASE(const_)
{
	std::vector<bool> serializeData = Function::serialize("1");

	Function f(serializeData);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 1);

	serializeData = Function::serialize("0");
	f = bcc::Function(serializeData);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string("11"))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("11"))), 0);

	serializeData = Function::serialize("1 + 0");
	f = bcc::Function(serializeData);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 1);

	serializeData = Function::serialize("1 + 1");
	f = bcc::Function(serializeData);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 0);
}

BOOST_AUTO_TEST_SUITE_END()
