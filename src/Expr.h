#ifndef EXPR_H_
#define EXPR_H_


#include "Node.h"

namespace bcc
{

class Expr: public Node
{
	public:
		enum Type
		{
			NONE,
			AND,
			OR,
			XOR,
			NOT
		};

		Expr(Expr::Type type);
		bool exec(const std::vector<bool> &) const throw(std::runtime_error);
		bool exec(const boost::dynamic_bitset<> &) const throw(std::runtime_error);

		Expr::Type    m_exprType;


};

}

#endif
