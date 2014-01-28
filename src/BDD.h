#ifndef BDD_H_
#define BDD_H_

#include <tuple>
#include <boost/dynamic_bitset.hpp>
#include <list>


typedef std::tuple<boost::dynamic_bitset<>, boost::dynamic_bitset<>, bool> Monom;

namespace bcc
{

class BDDNode
{
	public:
		std::auto_ptr<BDDNode>	     m_fixTrue;
		std::auto_ptr<BDDNode>	     m_fixFalse;
		virtual ~BDDNode() {};
};

class BDD
{
	public:
		BDD(const std::list<Monom> &monoms, bool constValue);
		bool exec(const boost::dynamic_bitset<> &);

	private:
		std::auto_ptr<BDDNode>      m_root;
};

}
#endif
