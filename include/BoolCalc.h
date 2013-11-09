#ifndef BOOL_CALC_H_
#define BOOL_CALC_H_

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include <boost/dynamic_bitset.hpp>

namespace bcc
{
	class Node;

	class Function
	{
		public:
			Function(const std::string &expression);
			~Function();
			bool calculate(const std::vector<bool> &values) const throw(std::runtime_error);
			bool calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error);
// | x0x1x2x3x4x5x6x7 | x8x9x10x11x12x13x14x15 | x16x17...
//			bool calculateRPN(const unsigned char *pvarValues) const;
			bool calculateRPN(const boost::dynamic_bitset<> &) const throw(std::runtime_error);

		protected:
			std::shared_ptr<bcc::Node>     m_root;
			void                           *m_pimpl;
	};
}

#endif
