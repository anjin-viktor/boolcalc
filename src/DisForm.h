#ifndef DIS_FORM_H_
#define DIS_FORM_H_

#include <vector>
#include <string>

#include <boost/dynamic_bitset.hpp>

namespace bcc
{

class Conjunct
{
	public:
		boost::dynamic_bitset<> m_pos;
		boost::dynamic_bitset<> m_neg;
};	


class DisForm
{
	public:
		std::vector<Conjunct>      m_conjuncts;
		std::string toString() const;

		static DisForm createFromStr(const std::string &df);
};


class BDDCalculator
{
	public:
		class Node
		{
			public:
				Node                     *m_fixTrue;
				Node                     *m_fixFalse;
				std::size_t               m_varId;
				bool                      m_value;
		};

		BDDCalculator();
		BDDCalculator(const DisForm &);
		BDDCalculator(const BDDCalculator &);

		BDDCalculator &operator=(const BDDCalculator &);

		~BDDCalculator();
		bool calculate(const boost::dynamic_bitset<> &) const;

	private:
		class Root
		{
			public:
				Node          *m_pnode;
				std::size_t    m_counter;
		};

		std::size_t findOptimalVar(const DisForm &);
		void processNode(Node *pnode, DisForm df);
		void deleteNode(Node *pnode);

		Root        *m_proot;
		
};

}




#endif
