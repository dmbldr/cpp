
#include <random>
#include <iostream>
#include <limits>

#include <gtest/gtest.h>

#include <big_integer.h>

TEST(construct_and_back, from_to_int)
{
    std::vector<int> values = {0, 999'999'999, 1'000'000'000, 1'000'010'090, 2'000'000'000, 2'000'010'090,
                              -1, -999'999'999,  -1'000'000'000, -1'000'010'090, -2'000'000'000, -2'000'010'090
    };

    for (auto val : values)
    {
        EXPECT_EQ(val, static_cast<int>(big_integer(val)));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(std::numeric_limits<int>::min());

    for (size_t i = 0; i < 1'000'000; ++i)
    {
        int val = dist(gen);
        EXPECT_EQ(val, big_integer(val));
    }
}

TEST(construct_and_back, from_to_string)
{
    EXPECT_EQ(1, 1);
}