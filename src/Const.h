#ifndef CONST_H_
#define CONST_H_


#include "Node.h"

namespace bcc
{

class Const: public Node
{
	public:
		Const(bool value);
		bool exec(const std::vector<bool> &) const throw(std::runtime_error);
		bool exec(const boost::dynamic_bitset<> &) const throw(std::runtime_error);

		bool     m_value;
};

}

#endif
