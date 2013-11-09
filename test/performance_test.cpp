#include <iostream>

#include <boost/chrono.hpp>

#include <BoolCalc.h>

#define MKSTR(x) #x


int main(int argc, char **argv)
{

	std::string func_str( 
		#include "func"
	);

	bcc::Function f(func_str);

	boost::dynamic_bitset<> v(std::string(
		"00010000100000000100000001000001000100000010000010000100000010001000000001000000000000100"));

	boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now();

	assert(f.calculate(v) == f.calculateRPN(v));
	v.flip();
	assert(f.calculate(v) == f.calculateRPN(v));

	for(std::size_t i=0; i<1000; i++)
		f.calculate(v);


    boost::chrono::nanoseconds duration = boost::chrono::high_resolution_clock::now() - start;
    std::cout << "\n\n================================================\n\n";
    std::cout << "Test 1 (repeated big function execution): " << duration.count() << " ns.\n\n";

	return 0;
}