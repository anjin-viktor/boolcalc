#ifndef BDD_H_
#define BDD_H_

#include <tuple>
#include <boost/dynamic_bitset.hpp>
#include <list>


typedef std::tuple<boost::dynamic_bitset<>, boost::dynamic_bitset<>, bool> Monom;

namespace bcc
{

struct BDDNode
{
	BDDNode                     *m_fixTrue;
	BDDNode                     *m_fixFalse;
	std::size_t                  m_varId;
	bool                         m_value;
	bool                         m_isTerm;
};

class BDD
{
	public:
		BDD(const std::list<Monom> &monoms, bool constValue);
		bool exec(const boost::dynamic_bitset<> &);
		~BDD();
	private:
		BDDNode         *m_root;
};

}
#endif
