#include "BDD.h"
#include <iostream>
namespace 
{
	class BDDNodeNonTerminal: public bcc::BDDNode
	{
		public:
			std::size_t    m_varId;
	};

	class BDDNodeTerminal: public bcc::BDDNode
	{
		public:
			bool         m_value;
	};
}


static std::size_t findMaxFreqVar(const std::list<Monom> &monoms)
{
	std::size_t monomSize = std::get<0>(*monoms.begin()).size();
	std::vector<std::size_t> freq(monomSize, 0);

	for(std::size_t i=0; i<monomSize; i++)
	{
		std::list<Monom>::const_iterator itr = monoms.begin();

		for(; itr != monoms.end(); itr++)
			if(std::get<0>(*itr).test(i) || std::get<1>(*itr).test(i))
				freq[i]++;
	}
	std::size_t position = std::max_element(freq.begin(), freq.end()) - freq.begin();

	return position;
}


bcc::BDDNode *createNode(const std::list<Monom> &monoms, bool constValue)
{
	if(monoms.empty())
	{
		BDDNodeTerminal *pterm = new BDDNodeTerminal;
		pterm-> m_value = constValue;

		return pterm;
	}

	std::size_t pos = findMaxFreqVar(monoms);
	std::list<Monom> fixTrue, fixFalse;
	bool constFixTrue = constValue;
	bool constFixFalse = constValue;

	std::list<Monom>::const_iterator itr = monoms.begin();

	for(; itr != monoms.end(); itr++)
	{
		Monom m = *itr;
		std::get<0>(m)[pos] = false;
		std::get<1>(m)[pos] = false;

		if(!std::get<1>(*itr).test(pos))
		{
//			if(std::get<0>(*itr).test(pos))
//			{
				if(!std::get<0>(m).any() && !std::get<1>(m).any())
					constFixTrue = !constFixTrue;
				else
					fixTrue.push_back(m);
//			}
//			else
		}
		if(!std::get<0>(*itr).test(pos))
		{
			if(!std::get<1>(m).any() && !std::get<0>(m).any() /*&& std::get<1>(*itr).test(pos)*/)
				constFixFalse = !constFixFalse;
			else
				fixFalse.push_back(m);
		}
	}

	BDDNodeNonTerminal *pnewNode = new BDDNodeNonTerminal;
	pnewNode -> m_varId = pos;

	if(!fixTrue.empty())
		pnewNode -> m_fixTrue.reset(createNode(fixTrue, constFixTrue));
	else
	{
		BDDNodeTerminal *pterm = new BDDNodeTerminal;
		pterm-> m_value = constFixTrue;
		pnewNode -> m_fixTrue.reset(pterm);
	}

	if(!fixFalse.empty())
		pnewNode -> m_fixFalse.reset(createNode(fixFalse, constFixFalse));
	else
	{
		BDDNodeTerminal *pterm = new BDDNodeTerminal;
		pterm-> m_value = constFixFalse;
		pnewNode -> m_fixFalse.reset(pterm);
	}

	return pnewNode;
}


bcc::BDD::BDD(const std::list<Monom> &monoms, bool constValue)
{
	m_root.reset(createNode(monoms, constValue));
}


bool bcc::BDD::exec(const boost::dynamic_bitset<> &vars)
{
	bcc::BDDNode *pnode = m_root.get();
	for(;;)
	{

		BDDNodeTerminal *pterm = dynamic_cast<BDDNodeTerminal *>(pnode);

		if(pterm)
			return pterm -> m_value;

		BDDNodeNonTerminal *pnonTerm = dynamic_cast<BDDNodeNonTerminal *>(pnode);
		if(vars.test(pnonTerm -> m_varId))
			pnode = pnode -> m_fixTrue.get();
		else
			pnode = pnode -> m_fixFalse.get();
	}

}
