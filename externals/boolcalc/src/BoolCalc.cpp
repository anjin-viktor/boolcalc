#include <BoolCalc.h>

#include "Node.h"
#include "BoolExprParser.h"

#include <boost/spirit/include/qi.hpp>

#include <list>
#include <utility>

typedef std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<> > Monom;

struct FunctionCalculatorImpl
{
	std::shared_ptr<bcc::Node>        m_root;
	std::list<Monom>                  m_monoms;
	bool                              m_constValue;
	bcc::Function::ExecutionType      m_execType;
};


void createMonom(const std::shared_ptr<bcc::Node> &node, Monom &monom)
{
	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = node -> m_childs.begin();

	for(;itr != node -> m_childs.end(); itr++)
	{
		const std::shared_ptr<bcc::Expr> expr = std::dynamic_pointer_cast<bcc::Expr>(*itr);
		const std::shared_ptr<bcc::Var> var = std::dynamic_pointer_cast<bcc::Var>(*itr);


		if(var)
			monom.first[var -> m_varId] = true;
		else if(expr)
		{
			if(expr -> m_exprType == bcc::Expr::NOT)
			{
				std::list<std::shared_ptr<bcc::Node> >::const_iterator itrNode = (*itr) -> m_childs.begin();
				if(std::dynamic_pointer_cast<bcc::Var>(*itrNode))
					monom.second[std::dynamic_pointer_cast<bcc::Var>(*itrNode) -> m_varId] = true;
				else 
					throw std::runtime_error("Expression is not ANF");
			}
			else if(expr -> m_exprType == bcc::Expr::AND)
				createMonom(*itr, monom);
			else
				throw std::runtime_error("Expression is not ANF");
		}
		else
			throw std::runtime_error("Expression is not ANF");
	}
}


void createListOfMonoms(const std::shared_ptr<bcc::Node> &root, std::list<Monom> &monoms, bool &constValue,
	std::size_t bitsSize)
{
	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = root -> m_childs.begin();

	if(itr == root -> m_childs.end())
	{
		if(std::dynamic_pointer_cast<bcc::Const>(root))
			constValue = std::dynamic_pointer_cast<bcc::Const>(root) -> m_value;
		else
		{
			const std::shared_ptr<bcc::Var> var = std::dynamic_pointer_cast<bcc::Var>(*itr);

			Monom monom;
			monom.first[var -> m_varId] = true;
			monoms.push_back(monom);
		}

	}

	for(;itr != root -> m_childs.end(); itr++)
	{
		const std::shared_ptr<bcc::Expr> expr = std::dynamic_pointer_cast<bcc::Expr>(*itr);
		const std::shared_ptr<bcc::Const> const_ = std::dynamic_pointer_cast<bcc::Const>(*itr);

		if(expr)
		{
			if(expr -> m_exprType == bcc::Expr::XOR)
				createListOfMonoms(*itr, monoms, constValue, bitsSize);
			else if(expr -> m_exprType == bcc::Expr::AND || expr -> m_exprType == bcc::Expr::NOT ||
				 std::dynamic_pointer_cast<bcc::Var>(*itr))
			{
				Monom monom;
				monom.first = boost::dynamic_bitset<>(bitsSize);
				monom.second = boost::dynamic_bitset<>(bitsSize);

				createMonom(*itr, monom);
				monoms.push_back(monom);
			}
			else 
				throw std::runtime_error("Expression is not ANF");
		}
		else if(const_)
		{
			if(const_ -> m_value)
				constValue = !constValue;
		}
	}
}


static void getBitsSize(const std::shared_ptr<bcc::Node> &node, std::size_t &size)
{
	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = node -> m_childs.begin();

	for(;itr != node -> m_childs.end(); itr++)
		getBitsSize(*itr, size);

	const std::shared_ptr<bcc::Var> pvar = std::dynamic_pointer_cast<bcc::Var>(node);

	if(pvar && (pvar -> m_varId + 1) > size)
		size = pvar -> m_varId + 1;
}


static bool execListOfMonoms(FunctionCalculatorImpl *m_pimpl, const boost::dynamic_bitset<> &values)
{
	std::list<Monom>::const_iterator itr = m_pimpl -> m_monoms.begin();

	bool result = m_pimpl -> m_constValue;

	for(;itr != m_pimpl -> m_monoms.end(); itr++)
	{
		if(itr -> first.is_subset_of(values) && !itr -> second.intersects(values))
			result = !result;
	}
	return result;
}


bcc::Function::Function(const std::string &expression, bcc::Function::ExecutionType type)
{
	std::string expr = expression;
	BoolExprParser<std::string::iterator> parser;
	std::string::iterator itr = expr.begin();
	bool res = boost::spirit::qi::parse(itr, expr.end(), parser);

	if(!res || itr != expr.end())
		throw std::runtime_error("expression `" + expression + "` is incorrect");

	FunctionCalculatorImpl *pimpl = new FunctionCalculatorImpl;
	m_pimpl = pimpl;

	pimpl -> m_root = parser.m_root;
	pimpl -> m_execType = type;
	pimpl -> m_constValue = false;

	if(type == LIST_OF_MONOMS)
	{
		std::size_t bitsSize = 0;
		getBitsSize(pimpl -> m_root, bitsSize);
		createListOfMonoms(pimpl -> m_root, pimpl -> m_monoms, pimpl -> m_constValue, bitsSize);
	}
}


bcc::Function::~Function()
{
	delete (FunctionCalculatorImpl *) m_pimpl;
}

const bcc::Function &bcc::Function::operator = (const Function &obj)
{
	delete (FunctionCalculatorImpl *) m_pimpl;
	m_pimpl = new FunctionCalculatorImpl;
	*((FunctionCalculatorImpl *) m_pimpl) = *((FunctionCalculatorImpl *)obj.m_pimpl);
}



bool bcc::Function::calculate(const std::vector<bool> &values) const throw(std::runtime_error)
{
	FunctionCalculatorImpl *pimpl = (FunctionCalculatorImpl *) m_pimpl;

	if(!pimpl -> m_root)
		throw std::runtime_error("expression is empty");

	if(pimpl -> m_execType == THREE)
		return pimpl -> m_root -> exec(values);
	else
	{
		boost::dynamic_bitset<> v(values.size());
		for(std::size_t i=0; i<values.size(); i++)
			if(values[i])
				v[i] = true;

		return execListOfMonoms(pimpl, v);
	}
}


bool bcc::Function::calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error)
{
	FunctionCalculatorImpl *pimpl = (FunctionCalculatorImpl *) m_pimpl;

	if(!pimpl)
		throw std::runtime_error("expression is empty");

	if(pimpl -> m_execType == THREE)
		return pimpl -> m_root -> exec(values);
	else
		return execListOfMonoms(pimpl, values);
}
