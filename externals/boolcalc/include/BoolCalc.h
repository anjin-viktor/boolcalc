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

			enum ExecutionType
			{
				THREE = 0,
				LIST_OF_MONOMS   // only for ANF
			};

			Function(const std::string &expression, ExecutionType type = THREE);
			virtual ~Function();

			const Function &operator = (const Function &);

			bool calculate(const std::vector<bool> &values) const throw(std::runtime_error);
			bool calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error);

		protected:
			void *m_pimpl;
	};
}

#endif
