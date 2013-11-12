#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/spirit/include/qi.hpp>
#include "BoolExprParser.h"

using namespace bcc;

BOOST_AUTO_TEST_SUITE(parser);

namespace qi = boost::spirit::qi;

BOOST_AUTO_TEST_CASE(parse)
{
	BoolExprParser<std::string::iterator> parser;


	std::string expr("x1&x0");
	std::string::iterator begin = expr.begin();

	bool res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & x0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & x0 + x2 | x3";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & (x0 + x2) | x3";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & ((x0 + x2)|x3)";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1 & x0 & x0 & x0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "x1";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "!x1";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());
	parser.clear();

	expr = "!0";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK_EQUAL(res, true);	
	BOOST_CHECK(begin == expr.end());
	parser.clear();


	expr = "x";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "()";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "x1 &";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "x1 & ()";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();

	expr = "x1 | x";
	begin = expr.begin();
	res = qi::parse(begin, expr.end(), parser);
	BOOST_CHECK(res == false || begin != expr.end());
	parser.clear();
}


BOOST_AUTO_TEST_CASE(parse_exec_elementary)
{
	std::shared_ptr<Node> and_, or_, xor_, not_;

	BoolExprParser<std::string::iterator> parser;

	std::string expr("x0 & x1");

	qi::parse(expr.begin(), expr.end(), parser);
	and_ = parser.m_root;
	parser.clear();

	expr = "x0 | x1";

	qi::parse(expr.begin(), expr.end(), parser);
	or_ = parser.m_root;
	parser.clear();

	expr ="x0 + x1";

	qi::parse(expr.begin(), expr.end(), parser);
	xor_ = parser.m_root;
	parser.clear();

	expr = "!x0";

	qi::parse(expr.begin(), expr.end(), parser);
	not_ = parser.m_root;
	parser.clear();

	std::vector<bool> v(2);
	v[0] = false;
	v[1] = false;

	BOOST_CHECK_EQUAL(and_ -> exec(v), false);
	BOOST_CHECK_EQUAL(or_ -> exec(v), false);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), false);
	BOOST_CHECK_EQUAL(not_ -> exec(v), true);

	v[0] = true;
	v[1] = false;

	BOOST_CHECK_EQUAL(and_ -> exec(v), false);
	BOOST_CHECK_EQUAL(or_ -> exec(v), true);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), true);
	BOOST_CHECK_EQUAL(not_ -> exec(v), false);

	v[0] = false;
	v[1] = true;

	BOOST_CHECK_EQUAL(and_ -> exec(v), false);
	BOOST_CHECK_EQUAL(or_ -> exec(v), true);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), true);
	BOOST_CHECK_EQUAL(not_ -> exec(v), true);

	v[0] = true;
	v[1] = true;

	BOOST_CHECK_EQUAL(and_ -> exec(v), true);
	BOOST_CHECK_EQUAL(or_ -> exec(v), true);
	BOOST_CHECK_EQUAL(xor_ -> exec(v), false);
	BOOST_CHECK_EQUAL(not_ -> exec(v), false);


}



BOOST_AUTO_TEST_CASE(parse_exec)
{

	BoolExprParser<std::string::iterator> parser;

	std::string expr("(!0 & x4) & ((x0 | x1) & (x2 + x3))");

	qi::parse(expr.begin(), expr.end(), parser);
	std::shared_ptr<Node> root = parser.m_root;
	parser.clear();

	std::vector<bool> v(5);

	v[0] = false;
	v[1] = false;
	v[2] = false;
	v[3] = false;
	v[4] = false;
	BOOST_CHECK_EQUAL(root -> exec(v), false);

	v[0] = true;
	v[1] = true;
	v[2] = true;
	v[3] = true;
	v[4] = true;
	BOOST_CHECK_EQUAL(root -> exec(v), false);

	v[0] = true;
	v[1] = false;
	v[2] = true;
	v[3] = false;
	v[4] = true;
	BOOST_CHECK_EQUAL(root -> exec(v), true);

	v[0] = false;
	v[1] = true;
	v[2] = false;
	v[3] = false;
	v[4] = true;
	BOOST_CHECK_EQUAL(root -> exec(v), false);

	v[0] = true;
	v[1] = true;
	v[2] = false;
	v[3] = true;
	v[4] = true;
	BOOST_CHECK_EQUAL(root -> exec(v), true);

}

BOOST_AUTO_TEST_SUITE_END()