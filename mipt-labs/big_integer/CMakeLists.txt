cmake_minimum_required (VERSION 3.16)
project (big_integer
	VERSION 1.0.0
	DESCRIPTION "Implementation of Big Integer library"
	LANGUAGES CXX)

add_library(big_integer)

set(sources
    include/big_integer.h
    src/big_integer.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

target_sources(big_integer PRIVATE ${sources})

target_include_directories(big_integer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(big_integer PROPERTIES
    CXX_STANDARD 17
    PUBLIC include/big_integer.h
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

enable_testing()
add_subdirectory(tests)