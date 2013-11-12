#include <iostream>
#include <vector>

#include <BoolCalc.h>


int main(int argc, char **argv)
{
	bcc::Function func("x1 & x0 | (x2 + 1)");
	static const bool v_[3] = {true, true, false};
	std::vector<bool> v(v_, v_ + sizeof(v_) / sizeof(v_[0]));

	std::cout << (int) func.calculate(v) << std::endl;
	return 0;
}
