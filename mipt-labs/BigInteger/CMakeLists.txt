cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME biginteger)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)


set(SOURCES
    BigInteger.cpp)


add_library(BigInteger )
