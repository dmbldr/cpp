cmake_minimum_required (VERSION 3.16)
project (big_integer
	VERSION 1.0.0
	DESCRIPTION "Implementation of Big Integer library"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(tests tests.cpp big_integer.cpp)
#----------------------------------------------------------------------------------------------------------------------
# testing framework
#----------------------------------------------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG        release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # do not override parent project's runtime settings on Windows
set(INSTALL_GTEST OFF)
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(googletest)

if (NOT MSVC)
	target_compile_options(tests PRIVATE -Wall -Wno-sign-compare -pedantic)
endif()

option(USE_SANITIZERS "Enable to build with undefined,leak and address sanitizers" OFF)
if (USE_SANITIZERS)
	target_compile_options(tests PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
	target_link_options(tests PUBLIC -fsanitize=address,undefined,leak)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(tests PUBLIC -stdlib=libc++)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_options(tests PUBLIC -D_GLIBCXX_DEBUG)
endif()

target_link_libraries(tests GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)