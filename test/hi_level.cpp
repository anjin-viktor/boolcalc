#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/dynamic_bitset.hpp>
#include "BoolCalc.h"
#include <iostream>
using namespace bcc;

BOOST_AUTO_TEST_SUITE(hi_level);


BOOST_AUTO_TEST_CASE(lili128_fd)
{
	bcc::Function fd(
			"x5 + x6 + x7 + x8 + x0 & x4 + x0 & x6 + x1 & x7"
			"+ x1 & x9 + x2 & x8 + x2 & x9 + x3 & x4 + x0 & x1 & x5"
			"+ x0 & x1 & x6 + x0 & x1 & x7 + x0 & x1 & x8 + x0 & x2 & x6"
			"+ x0 & x2 & x7 + x0 & x3 & x4 + x0 & x3 & x5 + x0 & x3 & x6"
			"+ x1 & x2 & x4 + x1 & x2 & x7 + x1 & x3 & x4 + x1 & x3 & x6"
			"+ x1 & x3 & x7 + x0 & x1 & x2 & x4 + x0 & x1 & x2 & x6"
			"+ x0 & x1 & x2 & x7 + x0 & x1 & x2 & x9 + x0 & x1 & x3 & x4"
			"+ x0 & x1 & x3 & x6 + x0 & x1 & x3 & x8 + x0 & x2 & x3 & x5"
			"+ x0 & x2 & x3 & x7 + x1 & x2 & x3 & x6 + x1 & x2 & x3 & x8"
			"+ x1 & x3 & x4 & x5 + x1 & x3 & x4 & x6 + x0 & x1 & x2 & x3 & x6"
			"+ x0 & x1 & x2 & x3 & x7 + x0 & x1 & x3 & x4 & x5"
			"+ x0 & x1 & x3 & x4 & x6 + x1 & x2 & x3 & x4 & x5"
			"+ x1 & x2 & x3 & x4 & x6 + x0 & x1 & x2 & x3 & x4 & x5"
			"+ x0 & x1 & x2 & x3 & x4 & x6"
		);

	bcc::Function fdAnf(
			"x5 + x6 + x7 + x8 + x0 & x4 + x0 & x6 + x1 & x7"
			"+ x1 & x9 + x2 & x8 + x2 & x9 + x3 & x4 + x0 & x1 & x5"
			"+ x0 & x1 & x6 + x0 & x1 & x7 + x0 & x1 & x8 + x0 & x2 & x6"
			"+ x0 & x2 & x7 + x0 & x3 & x4 + x0 & x3 & x5 + x0 & x3 & x6"
			"+ x1 & x2 & x4 + x1 & x2 & x7 + x1 & x3 & x4 + x1 & x3 & x6"
			"+ x1 & x3 & x7 + x0 & x1 & x2 & x4 + x0 & x1 & x2 & x6"
			"+ x0 & x1 & x2 & x7 + x0 & x1 & x2 & x9 + x0 & x1 & x3 & x4"
			"+ x0 & x1 & x3 & x6 + x0 & x1 & x3 & x8 + x0 & x2 & x3 & x5"
			"+ x0 & x2 & x3 & x7 + x1 & x2 & x3 & x6 + x1 & x2 & x3 & x8"
			"+ x1 & x3 & x4 & x5 + x1 & x3 & x4 & x6 + x0 & x1 & x2 & x3 & x6"
			"+ x0 & x1 & x2 & x3 & x7 + x0 & x1 & x3 & x4 & x5"
			"+ x0 & x1 & x3 & x4 & x6 + x1 & x2 & x3 & x4 & x5"
			"+ x1 & x2 & x3 & x4 & x6 + x0 & x1 & x2 & x3 & x4 & x5"
			"+ x0 & x1 & x2 & x3 & x4 & x6", 
			bcc::Function::LIST_OF_MONOMS
		);

	bcc::Function fdMap(
			"x5 + x6 + x7 + x8 + x0 & x4 + x0 & x6 + x1 & x7"
			"+ x1 & x9 + x2 & x8 + x2 & x9 + x3 & x4 + x0 & x1 & x5"
			"+ x0 & x1 & x6 + x0 & x1 & x7 + x0 & x1 & x8 + x0 & x2 & x6"
			"+ x0 & x2 & x7 + x0 & x3 & x4 + x0 & x3 & x5 + x0 & x3 & x6"
			"+ x1 & x2 & x4 + x1 & x2 & x7 + x1 & x3 & x4 + x1 & x3 & x6"
			"+ x1 & x3 & x7 + x0 & x1 & x2 & x4 + x0 & x1 & x2 & x6"
			"+ x0 & x1 & x2 & x7 + x0 & x1 & x2 & x9 + x0 & x1 & x3 & x4"
			"+ x0 & x1 & x3 & x6 + x0 & x1 & x3 & x8 + x0 & x2 & x3 & x5"
			"+ x0 & x2 & x3 & x7 + x1 & x2 & x3 & x6 + x1 & x2 & x3 & x8"
			"+ x1 & x3 & x4 & x5 + x1 & x3 & x4 & x6 + x0 & x1 & x2 & x3 & x6"
			"+ x0 & x1 & x2 & x3 & x7 + x0 & x1 & x3 & x4 & x5"
			"+ x0 & x1 & x3 & x4 & x6 + x1 & x2 & x3 & x4 & x5"
			"+ x1 & x2 & x3 & x4 & x6 + x0 & x1 & x2 & x3 & x4 & x5"
			"+ x0 & x1 & x2 & x3 & x4 & x6", 
			bcc::Function::MAP
		);

	std::vector<bool> v = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	BOOST_CHECK_NO_THROW(fd.calculate(v));
	BOOST_CHECK_EQUAL(fd.calculate(v), 0);
	BOOST_CHECK_NO_THROW(fdAnf.calculate(v));
	BOOST_CHECK_EQUAL(fdAnf.calculate(v), 0);
	BOOST_CHECK_NO_THROW(fdMap.calculate(v));
	BOOST_CHECK_EQUAL(fdMap.calculate(v), 0);


	v = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	BOOST_CHECK_NO_THROW(fd.calculate(v));
	BOOST_CHECK_EQUAL(fd.calculate(v), 0);
	BOOST_CHECK_NO_THROW(fdAnf.calculate(v));
	BOOST_CHECK_EQUAL(fdAnf.calculate(v), 0);
	BOOST_CHECK_NO_THROW(fdMap.calculate(v));
	BOOST_CHECK_EQUAL(fdMap.calculate(v), 0);


	v = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
	BOOST_CHECK_NO_THROW(fd.calculate(v));
	BOOST_CHECK_EQUAL(fd.calculate(v), 0);
	BOOST_CHECK_NO_THROW(fdAnf.calculate(v));
	BOOST_CHECK_EQUAL(fdAnf.calculate(v), 0);
	BOOST_CHECK_NO_THROW(fdMap.calculate(v));
	BOOST_CHECK_EQUAL(fdMap.calculate(v), 0);


	boost::dynamic_bitset<> x(10, 0x0AAAAA);
	BOOST_CHECK_NO_THROW(fd.calculate(x));
	BOOST_CHECK_EQUAL(fd.calculate(x), 1);
	BOOST_CHECK_NO_THROW(fdAnf.calculate(x));
	BOOST_CHECK_EQUAL(fdAnf.calculate(x), 1);
	BOOST_CHECK_NO_THROW(fdMap.calculate(x));
	BOOST_CHECK_EQUAL(fdMap.calculate(x), 1);
}



BOOST_AUTO_TEST_CASE(anf)
{
	bcc::Function f("1", bcc::Function::LIST_OF_MONOMS);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 1);


	f = bcc::Function("0", bcc::Function::LIST_OF_MONOMS);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string("11"))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("11"))), 0);

	f = bcc::Function("1 + 0", bcc::Function::LIST_OF_MONOMS);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 1);

	f = bcc::Function("1 + 1", bcc::Function::LIST_OF_MONOMS);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 0);
}




BOOST_AUTO_TEST_CASE(expr_1)
{
	bcc::Function f("(x0 | x1) & (x2 + x3) & !(x4 | x5)", bcc::Function::LIST_OF_MONOMS);

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


BOOST_AUTO_TEST_CASE(expr_not_monom)
{
	bcc::Function f1("!x0", bcc::Function::LIST_OF_MONOMS, 3);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("111"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("110"))), 1);

	bcc::Function f2("!(x0 & x1)", bcc::Function::LIST_OF_MONOMS);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("00"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("01"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("10"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("11"))), 0);

	bcc::Function f3("!(x0 & x1 & x2)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("000"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("001"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("010"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("011"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("100"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("101"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("110"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("111"))), 0);


	bcc::Function f4("!(x0 & x1 & x2 & x3)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0000"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0001"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0010"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0011"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0100"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0101"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0110"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("0111"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1000"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1001"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1010"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1011"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1100"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1101"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1110"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("1111"))), 0);

}


BOOST_AUTO_TEST_CASE(expr_not)
{
	bcc::Function f0("!(x0 + x1)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f0.calculate(boost::dynamic_bitset<>(std::string("00"))), 1);
	BOOST_CHECK_EQUAL(f0.calculate(boost::dynamic_bitset<>(std::string("01"))), 0);
	BOOST_CHECK_EQUAL(f0.calculate(boost::dynamic_bitset<>(std::string("10"))), 0);
	BOOST_CHECK_EQUAL(f0.calculate(boost::dynamic_bitset<>(std::string("11"))), 1);

	bcc::Function f1("!(x0 | x1)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("00"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("01"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("10"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("11"))), 0);

	bcc::Function f2("!(x0 + x1 + x2)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("000"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("001"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("010"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("011"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("100"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("101"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("110"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("111"))), 0);


	bcc::Function f3("!(x0 | x1 | x2)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("000"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("001"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("010"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("011"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("100"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("101"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("110"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("111"))), 0);


	bcc::Function f5("!(x0 + x1 + x2 + x3 | x4 | x5)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("000000"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("100000"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("100001"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("000011"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("000111"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("001111"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("011111"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("111111"))), 0);


}




BOOST_AUTO_TEST_CASE(expr_xor)
{
	bcc::Function f1("x0 + x1", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("10"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("01"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("11"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("00"))), 0);

	bcc::Function f2("x0 + x1 + x2", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("000"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("001"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("010"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("011"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("100"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("101"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("110"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("111"))), 1);

	bcc::Function f3("x0 + x1 + x2 + x3", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0000"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0001"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0010"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0011"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0100"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0101"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0110"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0111"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1000"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1001"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1010"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1011"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1100"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1101"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1110"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1111"))), 0);

	bcc::Function f4("!(x0 + x1)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("00"))), 1);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("01"))), 0);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("10"))), 0);
	BOOST_CHECK_EQUAL(f4.calculate(boost::dynamic_bitset<>(std::string("11"))), 1);

	bcc::Function f5("!(x0 + x1 + x2 + x3)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0000"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0001"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0010"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0011"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0100"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0101"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0110"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("0111"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1000"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1001"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1010"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1011"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1100"))), 1);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1101"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1110"))), 0);
	BOOST_CHECK_EQUAL(f5.calculate(boost::dynamic_bitset<>(std::string("1111"))), 1);

}


BOOST_AUTO_TEST_CASE(expr_or)
{
	bcc::Function f1("x0 | x1", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("10"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("01"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("11"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("00"))), 0);

	bcc::Function f2("x0 | x1 | x2", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("000"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("001"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("010"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("011"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("100"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("101"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("110"))), 1);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("111"))), 1);

	bcc::Function f3("x0 & x1 | x2 & x3", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0000"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0001"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0010"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0011"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0100"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0101"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0110"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("0111"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1000"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1001"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1010"))), 0);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1011"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1100"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1101"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1110"))), 1);
	BOOST_CHECK_EQUAL(f3.calculate(boost::dynamic_bitset<>(std::string("1111"))), 1);
}


BOOST_AUTO_TEST_CASE(expr_and)
{
	bcc::Function f1("x0 & x1", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("10"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("01"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("11"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("00"))), 0);

	bcc::Function f2("x0 & x1 & x2", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("000"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("001"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("010"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("011"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("100"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("101"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("110"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("111"))), 1);
}




BOOST_AUTO_TEST_CASE(expr_2)
{
	bcc::Function expr("(x0 | x1) & (x1 + x2) | x3 + x4 + 1", bcc::Function::LIST_OF_MONOMS);

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



BOOST_AUTO_TEST_CASE(expr_3)
{
	bcc::Function expr("(x0 | x1) & (x2 + x3)", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0000"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0001"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0010"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0011"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0100"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0101"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0111"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1000"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1001"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1010"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1011"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1100"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1101"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1110"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1111"))), 0);
}


BOOST_AUTO_TEST_CASE(expr_4)
{
	bcc::Function expr("0 + x0 | x1 & x2 + x3 | 0", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0000"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0001"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0010"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0011"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0100"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0101"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0111"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1000"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1001"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1010"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1011"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1100"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1101"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1110"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1111"))), 0);
}



BOOST_AUTO_TEST_CASE(diff_number_of_ops)
{
	bcc::Function expr("x0 + x1", bcc::Function::LIST_OF_MONOMS);

	BOOST_CHECK_NO_THROW(expr.calculate(boost::dynamic_bitset<>(std::string("0000"))));
	BOOST_CHECK_NO_THROW(expr.calculate(boost::dynamic_bitset<>(std::string("0"))));

	std::vector<bool> v1 = {0, 0, 0, 0};
	std::vector<bool> v2 = {0};

	BOOST_CHECK_NO_THROW(expr.calculate(v1));
	BOOST_CHECK_NO_THROW(expr.calculate(v2));
}



BOOST_AUTO_TEST_CASE(expr_4_BDD)
{
	bcc::Function expr("0 + x0 | x1 & x2 + x3 | 0", bcc::Function::BDD);

	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0000"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0001"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0010"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0011"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0100"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0101"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("0111"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1000"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1001"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1010"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1011"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1100"))), 1);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1101"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1110"))), 0);
	BOOST_CHECK_EQUAL(expr.calculate(boost::dynamic_bitset<>(std::string("1111"))), 0);
}


BOOST_AUTO_TEST_CASE(expr_and_bdd)
{
	bcc::Function f1("x0 & x1", bcc::Function::BDD);

	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("10"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("01"))), 0);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("11"))), 1);
	BOOST_CHECK_EQUAL(f1.calculate(boost::dynamic_bitset<>(std::string("00"))), 0);

	bcc::Function f2("x0 & x1 & x2", bcc::Function::BDD);

	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("000"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("001"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("010"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("011"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("100"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("101"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("110"))), 0);
	BOOST_CHECK_EQUAL(f2.calculate(boost::dynamic_bitset<>(std::string("111"))), 1);
}


BOOST_AUTO_TEST_CASE(anf_bdd)
{
	bcc::Function f("1", bcc::Function::BDD);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 1);


	f = bcc::Function("0", bcc::Function::BDD);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string("11"))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string("11"))), 0);

	f = bcc::Function("1 + 0", bcc::Function::BDD);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 1);

	f = bcc::Function("1 + 1", bcc::Function::BDD);
	BOOST_CHECK_NO_THROW(f.calculate(boost::dynamic_bitset<>(std::string(""))));
	BOOST_CHECK_EQUAL(f.calculate(boost::dynamic_bitset<>(std::string(""))), 0);
}




BOOST_AUTO_TEST_CASE(expr_1_bdd)
{
	bcc::Function f("(x0 | x1) & (x2 + x3) & !(x4 | x5)", bcc::Function::BDD);

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



BOOST_AUTO_TEST_SUITE_END()
