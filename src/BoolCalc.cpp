#include <BoolCalc.h>

#include "Node.h"
#include "BoolExprParser.h"

#include <boost/spirit/include/qi.hpp>

#include <list>
#include <utility>
#include <vector>

typedef std::pair<boost::dynamic_bitset<>, boost::dynamic_bitset<> > Monom;

struct FunctionCalculatorImpl
{
	std::shared_ptr<bcc::Node>               m_root;
	std::list<Monom>                         m_monoms;
	bool                                     m_constValue;
	bcc::Function::ExecutionType             m_execType;
	std::pair<std::vector<std::size_t>, 
	std::vector<bool> >                      m_values;
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
			const std::shared_ptr<bcc::Var> var = std::dynamic_pointer_cast<bcc::Var>(root);

			Monom monom;
			monom.first = boost::dynamic_bitset<>(bitsSize);
			monom.second = boost::dynamic_bitset<>(bitsSize);
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
		else
			createListOfMonoms(*itr, monoms, constValue, bitsSize);
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


static bool execListOfMonoms(FunctionCalculatorImpl *pimpl, const boost::dynamic_bitset<> &values)
{
	std::list<Monom>::const_iterator itr = pimpl -> m_monoms.begin();

	bool result = pimpl -> m_constValue;

	if(itr != pimpl -> m_monoms.end() && itr -> first.size() > values.size())
		throw std::runtime_error("vector of values is too smal");

	for(;itr != pimpl -> m_monoms.end(); itr++)
		if(itr -> first.is_subset_of(values) && !itr -> second.intersects(values))
			result = !result;

	return result;
}


static void getVars(const std::shared_ptr<bcc::Node> &node, std::vector<std::size_t> &res)
{
	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = node -> m_childs.begin();

	for(;itr != node -> m_childs.end(); itr++)
		getVars(*itr, res);

	const std::shared_ptr<bcc::Var> pvar = std::dynamic_pointer_cast<bcc::Var>(node);

	if(pvar)
		if(std::find(res.begin(), res.end(), pvar -> m_varId) == res.end())
			res.push_back(pvar -> m_varId);
}


static void createTable(std::pair<std::vector<std::size_t>, std::vector<bool> > &table,
	const std::shared_ptr<bcc::Node> &node)
{
	if(table.first.size() > sizeof(unsigned long) * 8)
		throw std::runtime_error("A lot of variables for `MAP` execution");
	table.second.resize(1 << table.first.size());
	unsigned long mask = 0;

	std::size_t size = *std::max_element(table.first.begin(), table.first.end()) + 1;
	boost::dynamic_bitset<> values(size);
	for(;mask < (1 << table.first.size()); mask++)
	{
		for(std::size_t i=0; i<table.first.size(); i++)
		{
			if(mask & (1 << i))
				values[table.first[i]] = 1;
			else
				values[table.first[i]] = 0;
		}

		table.second[mask] = node -> exec(values);
	}
}


template <typename T>
static bool execMap(FunctionCalculatorImpl *pimpl, const T& values)
{
	std::size_t position = 0;
	for(std::size_t i=0; i<pimpl -> m_values.first.size(); i++)
		if(values[pimpl -> m_values.first[i]])
			position |= 1 << i;

	return pimpl -> m_values.second[position];
}

bcc::Function::Function(const std::string &expression, bcc::Function::ExecutionType type, int monomSize)
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

		if(monomSize >= 0)
		{
			std::list<Monom>::iterator itr = pimpl -> m_monoms.begin();
			for(;itr != pimpl -> m_monoms.end(); itr++)
			{
				itr -> first.resize(monomSize);
				itr -> second.resize(monomSize);
			}
		}   
	}
	else if(type == MAP)
	{
		std::pair<std::vector<std::size_t>, std::vector<bool> > table;
		getVars(pimpl -> m_root, table.first);
		createTable(table, pimpl -> m_root);
		pimpl -> m_values = table;
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
	else if(pimpl -> m_execType == LIST_OF_MONOMS)
	{
		boost::dynamic_bitset<> v(values.size());
		for(std::size_t i=0; i<values.size(); i++)
			if(values[i])
				v[i] = true;

		return execListOfMonoms(pimpl, v);
	}
	else
		return execMap(pimpl, values);
}


bool bcc::Function::calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error)
{
	FunctionCalculatorImpl *pimpl = (FunctionCalculatorImpl *) m_pimpl;

	if(!pimpl)
		throw std::runtime_error("expression is empty");

	if(pimpl -> m_execType == THREE)
		return pimpl -> m_root -> exec(values);
	else if(pimpl -> m_execType == LIST_OF_MONOMS)
		return execListOfMonoms(pimpl, values);
	else
		return execMap(pimpl, values);
}


std::size_t bcc::Function::getNumberOfVars() const
{
        std::size_t bitsSize = 0;
        getBitsSize(((FunctionCalculatorImpl *) m_pimpl) -> m_root, bitsSize);
        return bitsSize;
}