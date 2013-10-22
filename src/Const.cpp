#include "Const.h"

Const::Const(bool value):
m_value(value)
{
}

bool Const::exec(const std::vector<bool> &) const throw(std::runtime_error)
{
	return m_value;
}