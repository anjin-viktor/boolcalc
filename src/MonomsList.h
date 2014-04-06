#ifndef MONOMS_LIST_H_
#define MONOMS_LIST_H_



#include <boost/dynamic_bitset.hpp>
#include <map>

namespace bcc
{
class DisForm;

class MonomsList
{
  public:
    void init(const bcc::DisForm &);
    bool calculate(const boost::dynamic_bitset<> &) const;

  private:
    std::map<boost::dynamic_bitset<>, bool>      m_monoms;
};

}


#endif
