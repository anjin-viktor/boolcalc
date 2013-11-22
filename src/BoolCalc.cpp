#include <BoolCalc.h>

#include "Node.h"
#include "BoolExprParser.h"

#include <boost/spirit/include/qi.hpp>

#include <list>
#include <utility>
#include <vector>
#include <tuple>

typedef std::tuple<boost::dynamic_bitset<>, boost::dynamic_bitset<>, bool, bcc::Expr::Type> Monom;

struct FunctionCalculatorImpl
{
	std::shared_ptr<bcc::Node>                                     m_root;
	std::list<Monom>                                               m_monoms;
	bcc::Function::ExecutionType                                   m_execType;
	std::pair<std::vector<std::size_t>, std::vector<bool> >        m_values;
};



void processNot(std::list<Monom> &monoms)
{
	std::list<Monom>::iterator itr = monoms.begin();

	if(monoms.size() == 1)
	{
		Monom tmp = *itr;
		monoms.clear();

		bool isFirstOperand = true;
		if(std::get<0>(tmp).any() || std::get<1>(tmp).any())
		{
			for(std::size_t i=0; i<std::get<0>(tmp).size(); i++)
			{
				if(std::get<0>(tmp).test(i))
				{
					Monom mon;
					std::get<0>(mon).resize(std::get<0>(tmp).size());
					std::get<1>(mon).resize(std::get<1>(tmp).size());
					std::get<1>(mon)[i] = 1;
					if(isFirstOperand)
						isFirstOperand = false;
					else
						std::get<3>(mon) = bcc::Expr::OR;
					monoms.push_back(mon);
				}
				else if(std::get<1>(tmp).test(i))
				{
					Monom mon;
					std::get<0>(mon).resize(std::get<0>(tmp).size());
					std::get<1>(mon).resize(std::get<1>(tmp).size());
					std::get<0>(mon)[i] = 1;
					if(isFirstOperand)
						isFirstOperand = false;
					else
						std::get<3>(mon) = bcc::Expr::OR;
					monoms.push_back(mon);
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
		std::cerr << "in processNot\n";
		Monom tmp = *itr;
/*		Monom notFirstOperand = firstOperand;
		boost::dynamic_bitset<> tmp = std::get<0>(notFirstOperand);
		std::get<0>(notFirstOperand) = std::get<1>(notFirstOperand);
		std::get<1>(notFirstOperand) = tmp;
		std::cerr << "tmp: " << std::get<1>(notFirstOperand) << std::endl;
		std::get<2>(notFirstOperand) = !std::get<2>(notFirstOperand);
*/
		std::list<Monom> firstOperand;
		firstOperand.push_back(tmp);
		processNot(firstOperand);

		monoms.erase(itr);

		std::list<Monom> result;
		switch(std::get<3>(*(monoms.begin())))
		{
			case bcc::Expr::OR:
			{
				processNot(monoms);

				std::list<Monom>::const_iterator itr = firstOperand.begin();
				for(;itr != firstOperand.end(); itr++)
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
								std::get<0>(m).clear();
								std::get<1>(m).clear();
								std::get<2>(m) = false;
							}
						}

						if(itr_ == monoms.begin())
							std::get<3>(m) = std::get<3>(*itr);
						else
							std::get<3>(m) = std::get<3>(*itr_);

						result.push_back(m);
					}
				}
				break;
			}
			case bcc::Expr::XOR:
			{
				itr = monoms.begin();
				for(;itr != monoms.end(); itr++)
				{
					Monom m;
					if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
					{
						if(std::get<2>(*itr))
							m = firstOperand;
						else
						{
							std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<1>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<2>(m) = false;
							std::get<3>(m) = std::get<3>(*itr);
						}
					}
					else if(std::get<0>(firstOperand).none() && std::get<1>(firstOperand).none())
					{
						if(std::get<2>(firstOperand))
							m = *itr;
						else
						{
							std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<1>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<2>(m) = false;
							std::get<3>(m) = std::get<3>(*itr);
						}
					}
					else
					{
						std::get<0>(m) = std::get<0>(*itr) | std::get<0>(firstOperand);
						std::get<1>(m) = std::get<1>(*itr) | std::get<1>(firstOperand);
						if(std::get<0>(m).intersects(std::get<1>(m)))
						{
							std::get<0>(m).clear();
							std::get<1>(m).clear();
							std::get<2>(m) = false;
						}
						std::get<3>(m) = std::get<3>(*itr);
					}
					result.push_back(m);
				}
				std::cerr << "xor 2\n";
				processNot(monoms);
				std::cerr << "xor 3\n";
				itr = monoms.begin();
				for(;itr != monoms.end(); itr++)
				{
					Monom m;
					if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
					{
						if(std::get<2>(*itr))
							m = notFirstOperand;
						else
						{
							std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<1>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<2>(m) = false;
							std::get<3>(m) = std::get<3>(*itr);
						}
					}
					else if(std::get<0>(notFirstOperand).none() && std::get<1>(notFirstOperand).none())
					{
						if(std::get<2>(notFirstOperand))
							m = *itr;
						else
						{
							std::get<0>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<1>(m) = boost::dynamic_bitset<>(std::get<0>(*itr).size());
							std::get<2>(m) = false;
							std::get<3>(m) = std::get<3>(*itr);
						}
					}
					else
					{
						std::get<0>(m) = std::get<0>(*itr) | std::get<0>(notFirstOperand);
						std::get<1>(m) = std::get<1>(*itr) | std::get<1>(notFirstOperand);
						if(std::get<0>(m).intersects(std::get<1>(m)))
						{
							std::get<0>(m).clear();
							std::get<1>(m).clear();
							std::get<2>(m) = false;
						}
						std::get<3>(m) = std::get<3>(*itr);
					}
					result.push_back(m);
				}
				std::cerr << "xor 4\n";
				break;
			}
		};
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
								std::get<0>(m).clear();
								std::get<1>(m).clear();
								std::get<2>(m) = false;
							}
						}

						if(itr_ == monoms_[1].begin())
							std::get<3>(m) = std::get<3>(*itr);
						else
							std::get<3>(m) = std::get<3>(*itr_);

						monoms.push_back(m);
					}
				}

				break;
			}

			case bcc::Expr::OR:
			{
				std::list<Monom>::iterator itr = monoms_[1].begin();

				for(;itr != monoms_[1].end(); itr++)
				{
					std::get<3>(*itr) = bcc::Expr::OR;
					monoms_[0].push_back(*itr);
				}
				monoms = monoms_[0];
				std::cerr << "size(OR): " << monoms.size() << std::endl;
				break;
			}

			case bcc::Expr::XOR:
			{
				std::list<Monom> notFirst = monoms_[0];
				std::list<Monom> notSecond = monoms_[1];
				std::cerr << "size(XOR): " << notFirst.size() << " " << notSecond.size() << std::endl;
				processNot(notFirst);
				processNot(notSecond);
				std::list<Monom>::const_iterator itr_ = notFirst.begin();

				std::cerr << "info: \n=====\n";

				for(;itr_ != notFirst.end(); itr_++)
				{
					std::cerr << std::get<0>(*itr_) << " " << std::get<1>(*itr_) << std::endl;
				}
				std::cerr << "=====\n";


//				std::cerr << monoms_[0] << " " << notFirst << " " << monoms_[1] << " " << notSecond << std::endl;
				std::list<Monom>::const_iterator itr = monoms_[0].begin();
				for(;itr != monoms_[0].end(); itr++)
				{
					std::list<Monom>::const_iterator itr_ = notSecond.begin();
					for(;itr_ != notSecond.end(); itr_++)
					{
						if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
						{
							if(std::get<2>(*itr))
								monoms.push_back(*itr_);
						}
						else if(std::get<0>(*itr_).none() && std::get<1>(*itr_).none())
						{
							if(std::get<2>(*itr_))
								monoms.push_back(*itr);
						}
						else
						{
							Monom m;
							std::get<0>(m) = std::get<0>(*itr) | std::get<0>(*itr_);
							std::get<1>(m) = std::get<1>(*itr) | std::get<1>(*itr_);

							if(std::get<0>(m).intersects(std::get<1>(m)))
							{
								std::get<0>(m).clear();
								std::get<1>(m).clear();
								std::get<2>(m) = false;
							}
							monoms.push_back(m);
						}	
					}
				}

				itr = monoms_[1].begin();
				for(;itr != monoms_[1].end(); itr++)
				{
					std::list<Monom>::const_iterator itr_ = notFirst.begin();
					for(;itr_ != notFirst.end(); itr_++)
					{
						if(std::get<0>(*itr).none() && std::get<1>(*itr).none())
						{
							if(std::get<2>(*itr))
								monoms.push_back(*itr_);
						}
						else if(std::get<0>(*itr_).none() && std::get<1>(*itr_).none())
						{
							if(std::get<2>(*itr_))
								monoms.push_back(*itr);
						}
						else
						{
							Monom m;
							std::get<0>(m) = std::get<0>(*itr) | std::get<0>(*itr_);
							std::get<1>(m) = std::get<1>(*itr) | std::get<1>(*itr_);

							if(std::get<0>(m).intersects(std::get<1>(m)))
							{
								std::get<0>(m).clear();
								std::get<1>(m).clear();
								std::get<2>(m) = false;
							}
							monoms.push_back(m);
						}
					}
				}



				break;
			}

			case bcc::Expr::NOT:
			{
				processNot(monoms_[0]);
				monoms = monoms_[0];
				std::cerr << "after not: " << std::get<1>(*(monoms.begin())) << std::endl;
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

	bool result;
	if(std::get<0>(*itr).any() || std::get<1>(*itr).any())
	{
		std::cerr << "has ";
		std::cerr << std::get<0>(*itr) << " " << std::get<1>(*itr) << " " << values << std::endl;
		if(std::get<0>(*itr).is_subset_of(values) && !std::get<1>(*itr).intersects(values))
			result = true;
		else
			result = false;
	}
	else
		result = std::get<2>(*itr);
std::cerr << "result: " << result << std::endl;
	for(itr++ ;itr != pimpl -> m_monoms.end(); itr++)
	{
		bool v;
		if(std::get<0>(*itr).any() || std::get<1>(*itr).any())
		{
			if(std::get<0>(*itr).is_subset_of(values) && !std::get<1>(*itr).intersects(values))
				v = true;
			else
				v = false;
		}
		else
			v = std::get<2>(*itr);

		if(std::get<3>(*itr) == bcc::Expr::OR)
		{
			std::cerr << "is or\n";
		std::cerr << std::get<0>(*itr) << " " << std::get<1>(*itr) << " " << values << std::endl;

			result = result | v;
		}
		else
		{
						std::cerr << "is xor\n";
		std::cerr << std::get<0>(*itr) << " " << std::get<1>(*itr) << " " << values << std::endl;

			result = result != v;
		}
std::cerr << "result: " << result << std::endl;

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

	if(type == LIST_OF_MONOMS)
	{
		std::size_t bitsSize = 0;
		getBitsSize(pimpl -> m_root, bitsSize);
		createListOfMonoms(pimpl -> m_root, pimpl -> m_monoms, bitsSize);
		if(monomSize >= 0)
		{
			std::list<Monom>::iterator itr = pimpl -> m_monoms.begin();
			for(;itr != pimpl -> m_monoms.end(); itr++)
			{
				std::get<0>(*itr).resize(monomSize);
				std::get<1>(*itr).resize(monomSize);
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