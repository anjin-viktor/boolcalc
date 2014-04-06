#include "MonomsList.h"

#include "DisForm.h"

using namespace bcc;
  
void MonomsList::init(const DisForm &df)
{
  m_monoms.clear();
  for(std::size_t i=0; i<df.m_conjuncts.size(); i++)
  {
    boost::dynamic_bitset<> bs = df.m_conjuncts[i].m_pos;
    m_monoms[bs] = true;
  }
}


bool MonomsList::calculate(const boost::dynamic_bitset<> &bs) const
{
  return m_monoms.find(bs) != m_monoms.end();
}