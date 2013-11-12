#include "Var.h"

#include <boost/lexical_cast.hpp>

using namespace bcc;

Var::Var(std::size_t varId):
m_varId(varId)
{
}


bool Var::exec(const std::vector<bool> &varValues) const throw(std::runtime_error)
{
	if(m_varId >= varValues.size())
	{
		throw std::runtime_error("Fault in calculate variable x" + boost::lexical_cast<std::string>(m_varId) +
			" because size of `values` vector is " +  
				boost::lexical_cast<std::string>(varValues.size()));
	}

	return varValues[m_varId];
}

bool Var::exec(const boost::dynamic_bitset<> &varValues) const throw(std::runtime_error)
{
	if(m_varId >= varValues.size())
	{
		throw std::runtime_error("Fault in calculate variable x" + boost::lexical_cast<std::string>(m_varId) +
			" because size of `values` vector is " +  
				boost::lexical_cast<std::string>(varValues.size()));
	}

	return varValues[m_varId];
}
