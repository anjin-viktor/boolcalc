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
				LIST_OF_MONOMS,   // only for ANF
				MAP
			};

			Function(const std::string &expression, ExecutionType type = THREE, int monomSize = -1);
			virtual ~Function();

			const Function &operator = (const Function &);

			bool calculate(const std::vector<bool> &values) const throw(std::runtime_error);
			bool calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error);

			std::size_t getNumberOfVars() const;

		protected:
			void *m_pimpl;
	};
}

#endif
