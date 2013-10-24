#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <list>
#include <memory>
#include <stdexcept>

namespace bcc
{

class Node
{
	public:
		virtual ~Node() {};
		virtual bool exec(const std::vector<bool> &) const throw(std::runtime_error) = 0;

		std::list<std::shared_ptr<Node> >   m_childs;
};

}

#endif
