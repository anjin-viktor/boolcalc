#include "Expr.h"

#include <boost/lexical_cast.hpp>

using namespace bcc;

Expr::Expr(Expr::Type type):
m_exprType(type)
{
}


bool Expr::exec(const std::vector<bool> &values) const throw(std::runtime_error)
{
	bool result;
	switch(m_exprType)
	{
		case Expr::AND:
		{
			if(m_childs.size() != 2)
			{
				throw std::runtime_error("Fault in calculate expession `&`: expected two operands, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = (*m_childs.begin()) -> exec(values) 
						&& (*(++m_childs.begin())) -> exec(values);
			break;
		}

		case Expr::OR:
		{
			if(m_childs.size() != 2)
			{
				throw std::runtime_error("Fault in calculate expession `|`: expected two operands, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = (*m_childs.begin()) -> exec(values) 
						|| (*(++m_childs.begin())) -> exec(values);
			break;
		}

		case Expr::XOR:
		{
			if(m_childs.size() != 2)
			{
				throw std::runtime_error("Fault in calculate expession `+`: expected two operands, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = (*m_childs.begin()) -> exec(values) 
						!= (*(++m_childs.begin())) -> exec(values);
			break;
		}

		case Expr::NOT:
		{
			if(m_childs.size() != 1)
			{
				throw std::runtime_error("Fault in calculate expession `!`: expected one operand, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = !(*m_childs.begin()) -> exec(values);
			break;
		}
	};

	return result;
}


bool Expr::exec(const boost::dynamic_bitset<> &values) const throw(std::runtime_error)
{
	bool result;
	switch(m_exprType)
	{
		case Expr::AND:
		{
			if(m_childs.size() != 2)
			{
				throw std::runtime_error("Fault in calculate expession `&`: expected two operands, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = (*m_childs.begin()) -> exec(values) 
						&& (*(++m_childs.begin())) -> exec(values);
			break;
		}

		case Expr::OR:
		{
			if(m_childs.size() != 2)
			{
				throw std::runtime_error("Fault in calculate expession `|`: expected two operands, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = (*m_childs.begin()) -> exec(values) 
						|| (*(++m_childs.begin())) -> exec(values);
			break;
		}

		case Expr::XOR:
		{
			if(m_childs.size() != 2)
			{
				throw std::runtime_error("Fault in calculate expession `+`: expected two operands, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = (*m_childs.begin()) -> exec(values) 
						!= (*(++m_childs.begin())) -> exec(values);
			break;
		}

		case Expr::NOT:
		{
			if(m_childs.size() != 1)
			{
				throw std::runtime_error("Fault in calculate expession `!`: expected one operand, but "
					" exists " + boost::lexical_cast<std::string>(m_childs.size()));
			}
			result = !(*m_childs.begin()) -> exec(values);
			break;
		}
	};

	return result;
}
