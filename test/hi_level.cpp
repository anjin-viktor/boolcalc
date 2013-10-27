#include <string>
#include <boost/test/unit_test.hpp>
#include "BoolCalc.h"

using namespace bcc;

BOOST_AUTO_TEST_SUITE(hi_level);


BOOST_AUTO_TEST_CASE(lili128_fd)
{
	bcc::Function fd(
			"x5 + x6 + x7 + x8 + x0 & x4 + x0 & x6 + x1 & x7"   //0
			"+ x1 & x9 + x2 & x8 + x2 & x9 + x3 & x4 + x0 & x1 & x5" // 1
			"+ x0 & x1 & x6 + x0 & x1 & x7 + x0 & x1 & x8 + x0 & x2 & x6" //0
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

	std::vector<bool> v = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	BOOST_CHECK_NO_THROW(fd.calculate(v));
	BOOST_CHECK_EQUAL(fd.calculate(v), 0);

	v = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	BOOST_CHECK_NO_THROW(fd.calculate(v));
	BOOST_CHECK_EQUAL(fd.calculate(v), 0);

	v = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
	BOOST_CHECK_NO_THROW(fd.calculate(v));
	BOOST_CHECK_EQUAL(fd.calculate(v), 0);


}



BOOST_AUTO_TEST_SUITE_END()