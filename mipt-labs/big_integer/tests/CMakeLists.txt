cmake_minimum_required (VERSION 3.16)
project(big_integer_tests)


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

#----------------------------------------------------------------------------------------------------------------------
# tests dependencies
#----------------------------------------------------------------------------------------------------------------------

#find_package(big_integer REQUIRED)


#----------------------------------------------------------------------------------------------------------------------
# tests sources
#----------------------------------------------------------------------------------------------------------------------

set(sources
    big_int_tests.cpp)

#----------------------------------------------------------------------------------------------------------------------
# tests target
#----------------------------------------------------------------------------------------------------------------------

add_executable(big_integer_tests)
target_sources(big_integer_tests PRIVATE ${sources})

target_include_directories(
    big_integer_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)
target_link_libraries(big_integer_tests
    PRIVATE
        big_integer
        gtest_main)

include(GoogleTest)
gtest_discover_tests(big_integer_tests)