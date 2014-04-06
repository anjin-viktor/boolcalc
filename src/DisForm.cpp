#include "DisForm.h"

#include <boost/lexical_cast.hpp>

using namespace bcc;

std::string DisForm::toString() const
{
	std::string res;

	for(std::size_t i=0; i<m_conjuncts.size(); i++)
	{
		if(!res.empty())
			res += " | ";
		bool needAnd = false;

		if(m_conjuncts[i].m_pos.test(0))
		{
			needAnd = true;
			res += "x0";
		}

		boost::dynamic_bitset<>::size_type pos = m_conjuncts[i].m_pos.find_next(0);
		for(;pos != boost::dynamic_bitset<>::npos; pos = m_conjuncts[i].m_pos.find_next(pos))
		{
			if(needAnd)
				res += " & ";

			needAnd = true;
			res += "x" + std::to_string(pos);
		}

		if(m_conjuncts[i].m_neg.test(0))
		{
			if(needAnd)
				res += " & ";

			needAnd = true;
			res += "!x0";
		}


		pos = m_conjuncts[i].m_neg.find_next(0);
		for(;pos != boost::dynamic_bitset<>::npos; pos = m_conjuncts[i].m_neg.find_next(pos))
		{
			if(needAnd)
				res += " & ";

			needAnd = true;
			res += "!x" + std::to_string(pos);
		}
	}

	return res;
}


BDDCalculator::BDDCalculator(const DisForm &df)
{
	m_proot = new BDDCalculator::Root;
	m_proot -> m_pnode = new BDDCalculator::Node;
	m_proot -> m_counter = 1;

	if(df.m_conjuncts.empty())
	{
		m_proot -> m_pnode -> m_value = false;
		m_proot -> m_pnode -> m_fixTrue = m_proot -> m_pnode -> m_fixFalse = NULL;
	}
	else
		processNode(m_proot -> m_pnode, df);
}


BDDCalculator::BDDCalculator():
	m_proot(NULL)
{
}


BDDCalculator::BDDCalculator(const BDDCalculator &obj)
{
	m_proot = obj.m_proot;
	m_proot -> m_counter++;
}


BDDCalculator::~BDDCalculator()
{
	if(m_proot)
	{
		m_proot -> m_counter--;
		if(m_proot -> m_counter == 0)
		{
			deleteNode(m_proot -> m_pnode);
			delete m_proot;
		}
	}
}


BDDCalculator &BDDCalculator::operator=(const BDDCalculator &obj)
{
	if(m_proot)
	{
		m_proot -> m_counter--;
		if(m_proot -> m_counter == 0)
		{
			deleteNode(m_proot -> m_pnode);
			delete m_proot;
		}
	}

	m_proot = obj.m_proot;
	m_proot -> m_counter++;

	return *this;
}


void BDDCalculator::deleteNode(BDDCalculator::Node *pnode)
{
	if(pnode -> m_fixTrue)
		deleteNode(pnode -> m_fixTrue);

	if(pnode -> m_fixFalse)
		deleteNode(pnode -> m_fixFalse);

	delete pnode;
}


void BDDCalculator::processNode(BDDCalculator::Node *pnode, DisForm df)
{
	pnode -> m_varId = findOptimalVar(df);

	DisForm dfFalse, dfTrue;

	bool hasTrue = false;
	bool hasFalse = false;
	for(std::size_t i=0; i<df.m_conjuncts.size(); i++)
	{
		Conjunct conj = df.m_conjuncts[i];
		conj.m_pos[pnode -> m_varId] = conj.m_neg[pnode -> m_varId] = false;
		bool conjIsEmpty = !conj.m_pos.any() && !conj.m_neg.any();

		if(df.m_conjuncts[i].m_pos.test(pnode -> m_varId))
		{
			if(!conjIsEmpty)
				dfTrue.m_conjuncts.push_back(conj);
			else
				hasTrue = true;
		}
		else if(df.m_conjuncts[i].m_neg.test(pnode -> m_varId))
		{
			if(!conjIsEmpty)
				dfFalse.m_conjuncts.push_back(conj);
			else
				hasFalse = true;
		}
		else
		{
			dfTrue.m_conjuncts.push_back(conj);
			dfFalse.m_conjuncts.push_back(conj);
		}
	}

	pnode -> m_fixTrue = new BDDCalculator::Node;
	pnode -> m_fixFalse = new BDDCalculator::Node;

	if(!hasTrue)
	{
		if(!dfTrue.m_conjuncts.empty())
			processNode(pnode -> m_fixTrue, dfTrue);
		else
		{
			pnode -> m_fixTrue -> m_fixTrue = pnode -> m_fixTrue -> m_fixFalse = NULL;
			pnode -> m_fixTrue -> m_value = false;
		}
	}
	else
	{
		pnode -> m_fixTrue -> m_fixTrue = pnode -> m_fixTrue -> m_fixFalse = NULL;
		pnode -> m_fixTrue -> m_value = true;
	}

	if(!hasFalse)
	{
		if(!dfFalse.m_conjuncts.empty())
			processNode(pnode -> m_fixFalse, dfFalse);
		else
		{
			pnode -> m_fixFalse -> m_fixTrue = pnode -> m_fixFalse -> m_fixFalse = NULL;
			pnode -> m_fixFalse -> m_value = false;
		}
	}
	else
	{
		pnode -> m_fixFalse -> m_fixTrue = pnode -> m_fixFalse -> m_fixFalse = NULL;
		pnode -> m_fixFalse -> m_value = true;
	}
}


std::size_t BDDCalculator::findOptimalVar(const DisForm &df)
{
	std::size_t conjSize = df.m_conjuncts[0].m_pos.size();
	std::vector<std::size_t> nums(conjSize, 0);
	for(std::size_t i=0; i<df.m_conjuncts.size(); i++)
	{
		for(std::size_t j=0; j<conjSize; j++)
			if(df.m_conjuncts[i].m_pos.test(j) || df.m_conjuncts[i].m_neg.test(j))
				nums[j]++;				
	}
	std::size_t position = std::max_element(nums.begin(), nums.end()) - nums.begin();
	return position;
}


bool BDDCalculator::calculate(const boost::dynamic_bitset<> &varValues) const
{
	if(!m_proot)
		return false;

	BDDCalculator::Node *pnode = m_proot -> m_pnode;

	if(!pnode)
		return false;


	for(;;)
	{
		if(pnode -> m_fixTrue && pnode -> m_fixFalse)
		{
			if(varValues.test(pnode -> m_varId))
				pnode = pnode -> m_fixTrue;
			else
				pnode = pnode -> m_fixFalse;
		}
		else
			return pnode -> m_value;
	}
}


DisForm DisForm::createFromStr(const std::string &str)
{
	DisForm result;
	std::size_t pos = 0;
	std::size_t monomSize = 0;

	for(;pos < str.length();)
	{
		std::size_t monomEndPos = str.find('|', pos);
		if(monomEndPos > str.length())
			monomEndPos = str.length();

		Conjunct conj;
		conj.m_pos.resize(monomSize);
		conj.m_neg.resize(monomSize);

		std::string m(str.substr(pos, monomEndPos - pos));
		bool isNot = false;
		for(std::size_t i=0; i<m.length(); i++)
		{
			if(m[i] == '!')
				isNot = true;
			if(m[i] == '&')
				isNot == false;
			if(isdigit(m[i]))
			{
				std::size_t k=1;
				while(isdigit(m[i + k]))
					k++;

				unsigned long long value = boost::lexical_cast<unsigned long long> (m.substr(i, k));
				if((value+1) > monomSize)
				{
					conj.m_pos.resize(value + 1);
					conj.m_neg.resize(value + 1);
					monomSize = value + 1;
				}

				if(isNot)
					conj.m_neg[value] = 1;
				else
					conj.m_pos[value] |= 1;

				i += k - 1;
			}
		}

		result.m_conjuncts.push_back(conj);
		pos = monomEndPos + 1;
	}

	for(std::size_t i=0; i<result.m_conjuncts.size(); i++)
	{
		result.m_conjuncts[i].m_pos.resize(monomSize);
		result.m_conjuncts[i].m_neg.resize(monomSize);
	}


	return result;
}

