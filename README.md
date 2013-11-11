boolcalc
========

BoolCalc is a library for calculate boolean expressions.


##USAGE

	#include <BoolCalc.h>
	...
	...
	...

	bcc::Function func(""x1 & x0 | (x2 + 1)"");
	static const bool v_[3] = {true, true, false};
	bool res = func.calculate(std::vector<bool>(v_, v_ + sizeof(v_) / sizeof(v_[0])));

	...

##COMPILING

Requements:

1) cmake

2) boost (header only)


	cmake ./
	make

Use `make test` for testing.
