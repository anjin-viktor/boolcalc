#include <BoolCalc.h>

#include "Node.h"
#include "BoolExprParser.h"
#include "BDD.h"

#include <boost/spirit/include/qi.hpp>

#include <list>
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::tuple<boost::dynamic_bitset<>, boost::dynamic_bitset<>, bool> Monom;

struct FunctionCalculatorImpl
{
	std::shared_ptr<bcc::Node>                                     m_root;
	std::list<Monom>                                               m_monoms;
	bool                                                           m_constValue;
	bcc::Function::ExecutionType                                   m_execType;
	std::pair<std::vector<std::size_t>, std::vector<bool> >        m_values;
	std::auto_ptr<bcc::BDD>                                        m_pBDD;
};



void processNot(std::list<Monom> &monoms)
{
	std::list<Monom>::iterator itr = monoms.begin();
	if(monoms.size() == 1)
	{
		Monom tmp = *itr;
		monoms.clear();
		if(std::get<0>(tmp).any() || std::get<1>(tmp).any())
		{
			std::vector<Monom> vars;
			for(std::size_t i=0; i<std::get<0>(tmp).size(); i++)
			{
				if(std::get<0>(tmp).test(i) && std::get<1>(tmp).test(i))
				{
					Monom mon;
					std::get<0>(mon).resize(std::get<0>(tmp).size());
					std::get<1>(mon).resize(std::get<1>(tmp).size());
					std::get<2>(mon) = 1;
					monoms.clear();
					monoms.push_back(mon);
					break;
				}
				else if(std::get<0>(tmp).test(i))
				{
					Monom mon;
					std::get<0>(mon).resize(std::get<0>(tmp).size());
					std::get<1>(mon).resize(std::get<1>(tmp).size());
					std::get<1>(mon)[i] = 1;
					monoms.push_back(mon);
					vars.push_back(mon);
				}
				else if(std::get<1>(tmp).test(i))
				{
					Monom mon;
					std::get<0>(mon).resize(std::get<0>(tmp).size());
					std::get<1>(mon).resize(std::get<1>(tmp).size());
					std::get<0>(mon)[i] = 1;
					monoms.push_back(mon);
					vars.push_back(mon);
				}
			}

			if(vars.size() > 1)
			{
				for(std::size_t k=2; k<=vars.size(); k++)
				{
					std::vector<bool> mask(vars.size());
					for(std::size_t i=1; i<=k; i++)
						mask[mask.size()-i] = 1;

					do
					{
						Monom mon;
						std::get<0>(mon).resize(std::get<0>(vars[0]).size());
						std::get<1>(mon).resize(std::get<1>(vars[0]).size());
						for(std::size_t i=0; i<mask.size(); i++)
						{
							if(mask[i])
							{
								for(std::size_t j=0; j<std::get<0>(vars[i]).size(); j++)
								{
									if(std::get<0>(vars[i]).test(j) && std::get<1>(mon).test(j) ||
										std::get<1>(vars[i]).test(j) && std::get<0>(mon).test(j))
									{
										std::get<0>(mon).clear();
										std::get<1>(mon).clear();
										break;
									}
									else if(std::get<0>(vars[i]).test(j))
										std::get<0>(mon)[j] = 1;
									else if(std::get<1>(vars[i]).test(j))
										std::get<1>(mon)[j] = 1;
								}


								if(!std::get<0>(mon).any() && !std::get<1>(mon).any())
									break;
							}
						}
						if(std::get<0>(mon).any() || std::get<1>(mon).any())
							monoms.push_back(mon);
					}
					while(std::next_permutation(mask.begin(), mask.end()));

				}
			}
		}
		else
		{
			Monom mon;
			std::get<0>(mon).resize(std::get<0>(tmp).size());
			std::get<1>(mon).resize(std::get<1>(tmp).size());
			std::get<2>(mon) = !std::get<2>(tmp);
			monoms.push_back(mon);
		}
	}
	else
	{
		Monom firstOperand = *itr;
		std::list<Monom> notFirstOperand;
		notFirstOperand.push_back(firstOperand);
		processNot(notFirstOperand);

		monoms.erase(itr);

		std::list<Monom> result;

		if(std::get<0>(firstOperand).none() && std::get<1>(firstOperand).none())
		{
			if(std::get<2>(firstOperand))
				result = monoms;
			else
			{
				Monom m;
				std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(firstOperand).size());
				std::get<1>(m) = boost::dynamic_bitset<>(std::get<1>(firstOperand).size());
				std::get<2>(m) = false;
				result.push_back(m);
			}
		}
		else
		{
			std::list<Monom>::const_iterator itr_ = monoms.begin();

			for(;itr_ != monoms.end(); itr_++)
			{
				Monom m;
				if(std::get<0>(*itr_).none() && std::get<1>(*itr_).none())
				{
					if(std::get<2>(*itr_))
					{
						m = firstOperand;
					}
					else
					{
						std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
						std::get<1>(m) = boost::dynamic_bitset<>(std::get<1>(*itr).size());
						std::get<2>(m) = false;
					}
				}
				else
				{
					std::get<0>(m) = std::get<0>(firstOperand) | std::get<0>(*itr_);
					std::get<1>(m) = std::get<1>(firstOperand) | std::get<1>(*itr_);
					if(std::get<0>(m).intersects(std::get<1>(m)))
					{
						std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(firstOperand).size());
						std::get<1>(m) = boost::dynamic_bitset<>(std::get<1>(firstOperand).size());
						std::get<2>(m) = false;
					}
				}
				if(std::find(result.begin(), result.end(), m) == result.end())
					result.push_back(m);
				else
					result.erase(std::find(result.begin(), result.end(), m));
			}
		}
		processNot(monoms);

		itr = notFirstOperand.begin();
		for(;itr != notFirstOperand.end(); itr++)
		{
			std::list<Monom>::const_iterator itr_ = monoms.begin();

			for(;itr_ != monoms.end(); itr_++)
			{
				Monom m;
				if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
				{
					if(std::get<2>(*itr))
						m = *itr_;
					else
					{
						std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
						std::get<1>(m) = boost::dynamic_bitset<>(std::get<1>(*itr).size());
						std::get<2>(m) = false;
					}
				}
				else if(std::get<0>(*itr_).none() && std::get<1>(*itr_).none())
				{
					if(std::get<2>(*itr_))
						m = *itr;
					else
					{
						std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
						std::get<1>(m) = boost::dynamic_bitset<>(std::get<1>(*itr).size());
						std::get<2>(m) = false;
					}
				}
				else
				{
					std::get<0>(m) = std::get<0>(*itr) | std::get<0>(*itr_);
					std::get<1>(m) = std::get<1>(*itr) | std::get<1>(*itr_);
					if(std::get<0>(m).intersects(std::get<1>(m)))
					{
						std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
						std::get<1>(m) = boost::dynamic_bitset<>(std::get<1>(*itr).size());
						std::get<2>(m) = false;
					}
				}
				if(std::find(result.begin(), result.end(), m) == result.end())
					result.push_back(m);
				else
					result.erase(std::find(result.begin(), result.end(), m));
			}
		}

		monoms = result;
	}
}


void createListOfMonoms(const std::shared_ptr<bcc::Node> &root, std::list<Monom> &monoms, 
	std::size_t bitsSize)
{
	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = root -> m_childs.begin();

	std::vector<std::list<Monom> > monoms_(root -> m_childs.size());
	for(std::size_t i=0; itr != root -> m_childs.end(); itr++, i++)
		createListOfMonoms(*itr, monoms_[i], bitsSize);

	const std::shared_ptr<bcc::Expr> expr = std::dynamic_pointer_cast<bcc::Expr>(root);
	const std::shared_ptr<bcc::Const> const_ = std::dynamic_pointer_cast<bcc::Const>(root);
	const std::shared_ptr<bcc::Var> var = std::dynamic_pointer_cast<bcc::Var>(root);

	if(var)
	{
		Monom m;
		std::get<0>(m) = boost::dynamic_bitset<>(bitsSize);
		std::get<1>(m) = boost::dynamic_bitset<>(bitsSize);
		std::get<0>(m).set(var -> m_varId);
		std::get<2>(m) = false;
		monoms.push_back(m);
	}
	else if(const_)
	{
		Monom m;
		std::get<0>(m) = boost::dynamic_bitset<>(bitsSize);
		std::get<1>(m) = boost::dynamic_bitset<>(bitsSize);
		std::get<2>(m) = const_ -> m_value;
		monoms.push_back(m);	
	}
	else if(expr)
	{
		switch(expr -> m_exprType)
		{
			case bcc::Expr::AND:
			{
				std::list<Monom>::const_iterator itr = monoms_[0].begin();
				for(;itr != monoms_[0].end(); itr++)
				{
					std::list<Monom>::const_iterator itr_ = monoms_[1].begin();
					for(;itr_ != monoms_[1].end(); itr_++)
					{
						Monom m;
						if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
						{
							if(std::get<2>(*itr))
								m = *itr_;
							else
							{
								std::get<0>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<1>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<2>(m) = false;
							}
						}
						else if(std::get<0>(*itr_).none() && std::get<1>(*itr_).none())
						{
							if(std::get<2>(*itr_))
								m = *itr;
							else
							{
								std::get<0>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<1>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<2>(m) = false;
							}
						}
						else
						{
							std::get<0>(m) = std::get<0>(*itr) | std::get<0>(*itr_);
							std::get<1>(m) = std::get<1>(*itr) | std::get<1>(*itr_);

							if(std::get<0>(m).intersects(std::get<1>(m)))
							{
								std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());;
								std::get<1>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());;
								std::get<2>(m) = false;
							}
						}

						monoms.push_back(m);
					}
				}

				break;
			}

			case bcc::Expr::OR:
			{
				monoms = monoms_[0];

				std::list<Monom>::iterator itr = monoms_[1].begin();
				for(;itr != monoms_[1].end(); itr++)
					monoms.push_back(*itr);

				itr = monoms_[0].begin();
				for(;itr != monoms_[0].end(); itr++)
				{
					std::list<Monom>::const_iterator itr_ = monoms_[1].begin();
					for(;itr_ != monoms_[1].end(); itr_++)
					{
						Monom m;
						if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
						{
							if(std::get<2>(*itr))
								m = *itr_;
							else
							{
								std::get<0>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<1>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<2>(m) = false;
							}
						}
						else if(std::get<0>(*itr_).none() && std::get<1>(*itr_).none())
						{
							if(std::get<2>(*itr_))
								m = *itr;
							else
							{
								std::get<0>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<1>(m) = boost::dynamic_bitset<>(bitsSize);
								std::get<2>(m) = false;
							}
						}
						else
						{
							std::get<0>(m) = std::get<0>(*itr) | std::get<0>(*itr_);
							std::get<1>(m) = std::get<1>(*itr) | std::get<1>(*itr_);

							if(std::get<0>(m).intersects(std::get<1>(m)))
							{
								std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
								std::get<1>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
								std::get<2>(m) = false;
							}
						}
						monoms.push_back(m);
					}
				}
				break;
			}

			case bcc::Expr::XOR:
			{
				std::list<Monom>::iterator itr = monoms_[1].begin();

				for(;itr != monoms_[1].end(); itr++)
					monoms_[0].push_back(*itr);

				monoms = monoms_[0];
				break;



				break;
			}

			case bcc::Expr::NOT:
			{
				processNot(monoms_[0]);
				monoms = monoms_[0];
				break;
			}
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


static bool execListOfMonoms(FunctionCalculatorImpl *pimpl, const boost::dynamic_bitset<> &values)
{
	std::list<Monom>::const_iterator itr = pimpl -> m_monoms.begin();
	bool result = pimpl -> m_constValue;
	for(; itr != pimpl -> m_monoms.end(); itr++)
	{
		if(std::get<0>(*itr).is_subset_of(values) && !std::get<1>(*itr).intersects(values))
			result = !result;
	}

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


bool execBDD(FunctionCalculatorImpl *pimpl, const boost::dynamic_bitset<> &values)
{
	return pimpl -> m_pBDD -> exec(values);
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

	if(type == LIST_OF_MONOMS || type == BDD)
	{
		pimpl -> m_constValue = false;
		std::size_t bitsSize = 0;
		getBitsSize(pimpl -> m_root, bitsSize);
		createListOfMonoms(pimpl -> m_root, pimpl -> m_monoms, bitsSize);


		std::list<Monom>::iterator itr = pimpl -> m_monoms.begin();

		for(;itr != pimpl -> m_monoms.end();)
		{
			std::size_t n = 0;
			std::list<Monom>::iterator itr_ = pimpl -> m_monoms.begin();

			for(;itr_ != pimpl -> m_monoms.end() && 
				(itr_ = std::find(itr_, pimpl -> m_monoms.end(), *itr)) != pimpl -> m_monoms.end(); itr_++)
				n++;

			if(n != 1)
			{

				Monom value = *itr;
				pimpl -> m_monoms.remove(*itr);

				if(n % 2)
					pimpl -> m_monoms.push_back(value);

				itr = pimpl -> m_monoms.begin();
			}
			else 
				itr++;
		}

		if(monomSize > 0)
		{
			itr = pimpl -> m_monoms.begin();
			for(;itr != pimpl -> m_monoms.end(); itr++)
			{
				std::get<0>(*itr).resize(monomSize);
				std::get<1>(*itr).resize(monomSize);
			}
		}

		itr = pimpl -> m_monoms.begin();

		for(;itr != pimpl -> m_monoms.end(); )
		{
			if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
			{

				if(std::get<2>(*itr))
					pimpl -> m_constValue = !(pimpl -> m_constValue);

				std::list<Monom>::iterator tmp = itr;
				tmp++;

				pimpl -> m_monoms.erase(itr);
				itr = tmp;
			}
			else
				itr++;
		}

		if(type == BDD)
		{
			pimpl -> m_pBDD.reset(new bcc::BDD(pimpl -> m_monoms, pimpl -> m_constValue));
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


bcc::Function::Function(const std::vector<bool> &serializeData)
{
	FunctionCalculatorImpl *pimpl = new FunctionCalculatorImpl;
	m_pimpl = pimpl;

	pimpl -> m_execType = LIST_OF_MONOMS;

	std::size_t numOfVars = 0;
	std::size_t pos = 0;

	for(; pos<31; pos++)
		if(serializeData[pos])
			numOfVars += std::pow(2, pos);

	pimpl -> m_constValue = serializeData[pos];
	pos++;

	for(;pos != serializeData.size();)
	{
		Monom monom;
		std::get<0>(monom).resize(numOfVars);
		std::get<1>(monom).resize(numOfVars);

		for(std::size_t i=0; i<numOfVars; i++)
		{
			if(serializeData[pos + i])
				std::get<0>(monom)[i] = 1;
			if(serializeData[pos + numOfVars + i])
				std::get<1>(monom)[i] = 1;
		}

		pimpl -> m_monoms.push_back(monom);
		pos += numOfVars * 2;
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
	else if(pimpl -> m_execType == LIST_OF_MONOMS || pimpl -> m_execType == BDD)
	{
		std::size_t size = values.size();
		if(!pimpl -> m_monoms.empty())
			size = std::get<0>(*(pimpl -> m_monoms.begin())).size();

		boost::dynamic_bitset<> v(size);
		for(std::size_t i=0; i<std::min(values.size(), size); i++)
			if(values[i])
				v[i] = true;

		if(pimpl -> m_execType == LIST_OF_MONOMS)
			return execListOfMonoms(pimpl, v);
		else
			return execBDD(pimpl, v);
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
	{
		if(!(pimpl -> m_monoms.empty()) && values.size() != std::get<0>(*(pimpl -> m_monoms.begin())).size())
		{
			boost::dynamic_bitset<> tmp = values;
			tmp.resize(std::get<0>(*(pimpl -> m_monoms.begin())).size());

			return execListOfMonoms(pimpl, tmp);
		}

		return execListOfMonoms(pimpl, values);
	}
	else if(pimpl -> m_execType == BDD)
		return execBDD(pimpl, values);
	else
		return execMap(pimpl, values);
}


std::size_t bcc::Function::getNumberOfVars() const
{
        std::size_t bitsSize = 0;
        getBitsSize(((FunctionCalculatorImpl *) m_pimpl) -> m_root, bitsSize);
        return bitsSize;
}



std::vector<bool> bcc::Function::serialize(const std::string &expression)
{
	bcc::Function f(expression, LIST_OF_MONOMS);
	std::size_t monomSize = f.getNumberOfVars();

	std::vector<bool> result;
	std::size_t i=0;
	for(;i < 31; monomSize /= 2, i++)
		result.push_back(monomSize % 2);

	result.push_back(((FunctionCalculatorImpl *) f.m_pimpl) -> m_constValue);
	monomSize = f.getNumberOfVars();

	std::list<Monom>::const_iterator itr = ((FunctionCalculatorImpl *) f.m_pimpl) -> m_monoms.begin();

	for(; itr != ((FunctionCalculatorImpl *) f.m_pimpl) -> m_monoms.end(); itr++)
	{
		boost::dynamic_bitset<> monom(monomSize * 2);
		for(std::size_t i=0; i<monomSize; i++)
		{
			if(std::get<0>(*itr)[i])
				monom[i] = 1;
			if(std::get<1>(*itr)[i])
				monom[monomSize + i] = 1;
		}

		for(std::size_t i=0; i<monomSize * 2; i++)
			result.push_back(monom[i]);

	}
	return result;
}