#ifndef VAR_H_
#define VAR_H_


#include "Node.h"


namespace bcc
{

class Var: public Node
{
	public:
		Var(std::size_t varId);
		bool exec(const std::vector<bool> &) const throw(std::runtime_error);
		bool exec(const boost::dynamic_bitset<> &) const throw(std::runtime_error);

		std::size_t      m_varId;
};

}

#endif
