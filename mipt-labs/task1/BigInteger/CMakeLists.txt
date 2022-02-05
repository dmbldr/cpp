cmake_minimum_required(VERSION 3.8)
project(BigInteger)

add_library (BigInteger STATIC  "BigInteger.cpp" "impl.cpp")
set_target_properties(
	BigInteger PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
