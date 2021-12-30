#include "BigInteger.h"

#include <iostream>


int main() {
	BigInt x1 = 2;
	BigInt x2 = 2;
	x1 += x2;
	std::cout << "BigInt: " << x1 << std::endl;
	return 0;
}