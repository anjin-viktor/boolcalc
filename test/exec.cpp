#include <memory>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "Node.h"
#include "Var.h"
#include "Const.h"
#include "Expr.h"

using namespace bcc;

BOOST_AUTO_TEST_SUITE(exec);

BOOST_AUTO_TEST_CASE(AND)
{
	Expr expr(Expr::AND);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);
}



BOOST_AUTO_TEST_CASE(OR)
{
	Expr expr(Expr::OR);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);
}



BOOST_AUTO_TEST_CASE(XOR)
{
	Expr expr(Expr::XOR);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);
}



BOOST_AUTO_TEST_CASE(NOT)
{
	Expr expr(Expr::NOT);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(1)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), false);

	expr.m_childs.clear();
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_EQUAL(expr.exec(std::vector<bool>()), true);

	expr.m_childs.clear();
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);

	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	expr.m_childs.push_back(std::shared_ptr<Node>(new Const(0)));
	BOOST_CHECK_THROW(expr.exec(std::vector<bool>()), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(VARIABLE)
{
	Var var0(0), var1(1), var2(2);

	static const bool values_[] = {false, true};
	std::vector<bool> values(values_, values_ + sizeof(values_) / sizeof(values_[0]));

	BOOST_CHECK_EQUAL(var0.exec(values), false);
	BOOST_CHECK_EQUAL(var1.exec(values), true);
	BOOST_CHECK_THROW(var2.exec(values), std::runtime_error);
}



BOOST_AUTO_TEST_CASE(TREE)
{
	std::shared_ptr<Node> root(new Expr(Expr::OR));
	std::shared_ptr<Node> xor1(new Expr(Expr::XOR));
	std::shared_ptr<Node> xor2(new Expr(Expr::XOR));
	std::shared_ptr<Node> or1(new Expr(Expr::OR));
	std::shared_ptr<Node> and1(new Expr(Expr::AND));
	std::shared_ptr<Node> and2(new Expr(Expr::AND));
	std::shared_ptr<Node> not1(new Expr(Expr::NOT));

	or1 -> m_childs.push_back(std::shared_ptr<Node>(new Var(0)));
	or1 -> m_childs.push_back(std::shared_ptr<Node>(new Var(1)));

	and1 -> m_childs.push_back(std::shared_ptr<Node>(new Var(1)));
	and1 -> m_childs.push_back(std::shared_ptr<Node>(new Var(2)));

	xor2 -> m_childs.push_back(std::shared_ptr<Node>(new Var(3)));
	xor2 -> m_childs.push_back(std::shared_ptr<Node>(new Var(2)));

	not1 -> m_childs.push_back(std::shared_ptr<Node>(new Var(2)));

	xor1 -> m_childs.push_back(or1);
	xor1 -> m_childs.push_back(and1);

	and2 -> m_childs.push_back(xor2);
	and2 -> m_childs.push_back(not1);

	root -> m_childs.push_back(xor1);
	root -> m_childs.push_back(and2);

	std::vector<bool> values(4);

	values[0] = false;
	values[1] = false;
	values[2] = false;
	values[3] = false;

	BOOST_CHECK_EQUAL(root -> exec(values), false);


	values[0] = false;
	values[1] = false;
	values[2] = false;
	values[3] = true;

	BOOST_CHECK_EQUAL(root -> exec(values), true);

	values[0] = true;
	values[1] = false;
	values[2] = false;
	values[3] = true;

	BOOST_CHECK_EQUAL(root -> exec(values), true);

	values[0] = true;
	values[1] = true;
	values[2] = true;
	values[3] = true;

	BOOST_CHECK_EQUAL(root -> exec(values), false);

}

BOOST_AUTO_TEST_SUITE_END();