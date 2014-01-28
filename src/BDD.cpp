#include "BDD.h"
#include <iostream>

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
		bcc::BDDNode *pterm = new bcc::BDDNode;
		pterm -> m_value = constValue;
		pterm -> m_isTerm = true;
		pterm -> m_fixFalse = pterm -> m_fixTrue = NULL;


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
				if(!std::get<0>(m).any() && !std::get<1>(m).any())
					constFixTrue = !constFixTrue;
				else
					fixTrue.push_back(m);
		}
		if(!std::get<0>(*itr).test(pos))
		{
			if(!std::get<1>(m).any() && !std::get<0>(m).any())
				constFixFalse = !constFixFalse;
			else
				fixFalse.push_back(m);
		}
	}

	bcc::BDDNode *pnewNode = new bcc::BDDNode;
	pnewNode -> m_varId = pos;
	pnewNode -> m_isTerm = false;

	if(!fixTrue.empty())
		pnewNode -> m_fixTrue = createNode(fixTrue, constFixTrue);
	else
	{
		bcc::BDDNode *pterm = new bcc::BDDNode;
		pterm -> m_value = constFixTrue;
		pterm -> m_isTerm = true;
		pterm -> m_fixFalse = pterm -> m_fixTrue = NULL;

		pnewNode -> m_fixTrue = pterm;
	}

	if(!fixFalse.empty())
		pnewNode -> m_fixFalse = createNode(fixFalse, constFixFalse);
	else
	{
		bcc::BDDNode *pterm = new bcc::BDDNode;
		pterm-> m_value = constFixFalse;
		pterm -> m_isTerm = true;
		pterm -> m_fixFalse = pterm -> m_fixTrue = NULL;
		pnewNode -> m_fixFalse = pterm;
	}

	return pnewNode;
}


static void deleteNode(bcc::BDDNode *pnode)
{
	if(pnode -> m_fixTrue)
		deleteNode(pnode -> m_fixTrue);

	if(pnode -> m_fixFalse)
		deleteNode(pnode -> m_fixFalse);

	delete pnode;
}


bcc::BDD::BDD(const std::list<Monom> &monoms, bool constValue)
{
	m_root = createNode(monoms, constValue);
}


bcc::BDD::~BDD()
{
	deleteNode(m_root);
}

bool bcc::BDD::exec(const boost::dynamic_bitset<> &vars)
{
	bcc::BDDNode *pnode = m_root;
	for(;!pnode -> m_isTerm;)
	{
		if(vars.test(pnode -> m_varId))
			pnode = pnode -> m_fixTrue;
		else
			pnode = pnode -> m_fixFalse;
	}

	return pnode -> m_value;
}
