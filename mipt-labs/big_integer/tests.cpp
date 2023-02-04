
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "gtest/gtest.h"

#include "big_integer.h"

TEST(correctness, from_int)
{
    std::vector<int> values = {0, 999'999'999, 1'000'000'000, 1'000'010'090, 2'000'000'000, 2'000'010'090,
                              -1, -999'999'999,  -1'000'000'000, -1'000'010'090, -2'000'000'000, -2'000'010'090
    };

    for (auto val : values)
    {
        EXPECT_EQ(std::to_string(val), to_string(big_integer(val)));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> dist(std::numeric_limits<int64_t>::min());

    for (size_t i = 0; i < 1'000'000; ++i)
    {
        int64_t val = dist(gen);
        EXPECT_EQ(std::to_string(val), to_string(big_integer(val)));
    }
}

TEST(correctness, two_plus_two) {
    EXPECT_EQ(big_integer(4), big_integer(2) + big_integer(2));
    EXPECT_EQ(4, big_integer(2) + 2); // implicit conversion from int must work
    EXPECT_EQ(4, 2 + big_integer(2));
}

TEST(correctness, default_ctor) {
    big_integer a;
    big_integer b = 0;
    EXPECT_EQ(0, a);
    EXPECT_EQ(b, a);
}

TEST(correctness, ctor_limits) {
    big_integer a = std::numeric_limits<int>::min();
    big_integer b = std::numeric_limits<int>::max();
    EXPECT_EQ(-1, a + b);
}

TEST(correctness, copy_ctor) {
    big_integer a = 3;
    big_integer b = a;

    EXPECT_EQ(b, a);
    EXPECT_EQ(3, b);
}

TEST(correctness, copy_ctor_real_copy) {
    big_integer a = 3;
    big_integer b = a;
    a = 5;

    EXPECT_EQ(3, b);
}

TEST(correctness, copy_ctor_real_copy2) {
    big_integer a = 3;
    big_integer b = a;
    b = 5;

    EXPECT_EQ(3, a);
}

TEST(correctness, ctor_invalid_string) {
    EXPECT_THROW(big_integer("abc"), std::invalid_argument);
    EXPECT_THROW(big_integer("123x"), std::invalid_argument);
    EXPECT_THROW(big_integer(""), std::invalid_argument);
    EXPECT_THROW(big_integer("-"), std::invalid_argument);
    EXPECT_THROW(big_integer("-x"), std::invalid_argument);
    EXPECT_THROW(big_integer("123-456"), std::invalid_argument);
    EXPECT_THROW(big_integer("--5"), std::invalid_argument);
    EXPECT_THROW(big_integer("++5"), std::invalid_argument);
}

TEST(correctness, assignment_operator) {
    big_integer a = 4;
    big_integer b = 7;
    b = a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, self_assignment) {
    big_integer a = 5;
    a = a;

    EXPECT_TRUE(a == 5);
}

TEST(correctness, assignment_return_value) {
    big_integer a = 4;
    big_integer b = 7;
    (a = b) = a;

    EXPECT_TRUE(a == 7);
    EXPECT_TRUE(b == 7);
}

TEST(correctness, comparisons) {
    big_integer a = 100;
    big_integer b = 100;
    big_integer c = 200;
    big_integer d = -1;

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(c >= a);
    EXPECT_TRUE(d != 0);
}

TEST(correctness, compare_zero_and_minus_zero) {
    big_integer a;
    big_integer b = -a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, add) {
    big_integer a = 5;
    big_integer b = 20;

    EXPECT_TRUE(a + b == 25);

    a += b;
    EXPECT_TRUE(a == 25);
}

TEST(correctness, add_signed) {
    big_integer a = 5;
    big_integer b = -20;
    EXPECT_TRUE(a + b == -15);

    a += b;
    EXPECT_TRUE(a == -15);
}

TEST(correctness, add_return_value) {
    big_integer a = 5;
    big_integer b = 1;

    (a += b) += b;
    EXPECT_EQ(7, a);
}

TEST(correctness, sub) {
    big_integer a = 20;
    big_integer b = 5;

    EXPECT_TRUE(a - b == 15);

    a -= b;
    EXPECT_TRUE(a == 15);
}

TEST(correctness, sub_signed) {
    big_integer a = 5;
    big_integer b = 20;

    EXPECT_TRUE(a - b == -15);

    a -= b;
    EXPECT_TRUE(a == -15);

    a -= -100;
    EXPECT_TRUE(a == 85);
}

TEST(correctness, sub_return_value) {
    big_integer a = 5;
    big_integer b = 1;

    (a -= b) -= b;
    EXPECT_EQ(3, a);
}

TEST(correctness, mul) {
    big_integer a = 5;
    big_integer b = 20;
    EXPECT_TRUE(a * b == 100);

    a *= b;
    EXPECT_TRUE(a == 100);
}

TEST(correctness, mul_signed) {
    big_integer a = -5;
    big_integer b = 20;

    EXPECT_TRUE(a * b == -100);

    a *= b;
    EXPECT_TRUE(a == -100);
}

TEST(correctness, mul_return_value) {
    big_integer a = 5;
    big_integer b = 2;

    (a *= b) *= b;
    EXPECT_EQ(20, a);
}

TEST(correctness, div_) {
    big_integer a = 20;
    big_integer b = 5;
    big_integer c = 20;
    EXPECT_EQ(0, b / c);
    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 0);

    a /= b;
    EXPECT_TRUE(a == 4);

    c %= b;
    EXPECT_TRUE(c == 0);
}

TEST(correctness, div_int_min) {
    big_integer a = std::numeric_limits<int>::min();
    EXPECT_TRUE((a / a) == (a / std::numeric_limits<int>::min()));
}

TEST(correctness, div_int_min_2) {
    big_integer a = std::numeric_limits<int>::min();
    big_integer b = -1;
    big_integer c = a / b;
    EXPECT_TRUE(c == (a / -1));
    EXPECT_TRUE((c - std::numeric_limits<int>::max()) == 1);
}

TEST(correctness, div_signed) {
    big_integer a = -20;
    big_integer b = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(a % b == 0);
}

TEST(correctness, div_rounding) {
    big_integer a = 23;
    big_integer b = 5;

    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 3);
}

TEST(correctness, div_rounding_negative) {
    big_integer a = 23;
    big_integer b = -5;
    big_integer c = -23;
    big_integer d = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(c / d == -4);
    EXPECT_TRUE(a % b == 3);
    EXPECT_TRUE(c % d == -3);
}

TEST(correctness, div_return_value) {
    big_integer a = 100;
    big_integer b = 2;

    (a /= b) /= b;
    EXPECT_EQ(25, a);
}

TEST(correctness, unary_plus) {
    big_integer a = 123;
    big_integer b = +a;

    EXPECT_TRUE(a == b);

    // this code should not compile:
    // &+a;
}

TEST(correctness, negation) {
    big_integer a = 666;
    big_integer b = -a;

    EXPECT_TRUE(b == -666);
}

TEST(correctness, negation_int_min) {
    big_integer a = std::numeric_limits<int>::min();
    big_integer b = -a;

    EXPECT_EQ(b - 1, std::numeric_limits<int>::max());
}

TEST(correctness, increment) {
    big_integer a = 42;
    big_integer pre = ++a;
    big_integer post = a++;

    EXPECT_EQ(43, pre);
    EXPECT_EQ(43, post);
}

TEST(correctness, decrement) {
    big_integer a = 42;
    big_integer pre = --a;
    big_integer post = a--;

    EXPECT_EQ(41, pre);
    EXPECT_EQ(41, post);
}

TEST(correctness, add_long) {
    big_integer a("10000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("10000000000000000000000000000000000000000000000000000100000000"
                  "000000000000000000000000000000");

    EXPECT_EQ(c, a + b);
}

TEST(correctness, add_long_signed) {
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("10000000000000000000000000000000000000000000000000000000000000"
                  "00000000000000000000000000000");

    EXPECT_EQ(0, a + b);
}

TEST(correctness, add_long_signed2) {
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("-9999999999999999999999999999999999999999999999999999000000000"
                  "00000000000000000000000000000");

    EXPECT_EQ(c, a + b);
}

TEST(correctness, add_long_pow2) {
    big_integer a("18446744073709551616");
    big_integer b("-18446744073709551616");
    big_integer c("36893488147419103232");

    EXPECT_EQ(c, a + a);
    EXPECT_EQ(a, b + c);
    EXPECT_EQ(a, c + b);
}

TEST(correctness, sub_long) {
    big_integer a("10000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("99999999999999999999999999999999999999999999999999999000000000"
                  "00000000000000000000000000000");

    EXPECT_EQ(c, a - b);
}

TEST(correctness, sub_long_pow2) {
    big_integer a("36893488147419103232");
    big_integer b("36893488147419103231");

    EXPECT_EQ(1, a - b);
}

TEST(correctness, mul_long) {
    big_integer a("10000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("10000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000"
                  "00000000000000000000000000000000000000");

    EXPECT_EQ(c, a * b);
}

TEST(correctness, mul_long_signed) {
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("-1000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000"
                  "00000000000000000000000000000000000000");

    EXPECT_EQ(c, a * b);
}

TEST(correctness, mul_long_signed2) {
    big_integer a("-100000000000000000000000000");
    big_integer c("100000000000000000000000000"
                  "00000000000000000000000000");

    EXPECT_EQ(c, a * a);
}

TEST(correctness, mul_long_pow2) {
    big_integer a("18446744073709551616");
    big_integer b("340282366920938463463374607431768211456");
    big_integer c("11579208923731619542357098500868790785326998466564056403945758"
                  "4007913129639936");

    EXPECT_EQ(b, a * a);
    EXPECT_EQ(c, b * b);
}

TEST(correctness, div_0_long) {
    big_integer a;
    big_integer b("100000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(a, a / b);
}

TEST(correctness, div_long) {
    big_integer a("10000000000000000000000000000000000000000000000000000000000000"
                  "000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(correctness, div_long_signed) {
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000"
                  "0000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("-100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(correctness, div_long_signed2) {
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000"
                  "0000000000000000000000000000000");
    big_integer b("-100000000000000000000000000000000000000");
    big_integer c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(correctness, negation_long) {
    big_integer a("10000000000000000000000000000000000000000000000000000");
    big_integer c("-10000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(-a, c);
    EXPECT_EQ(a, -c);
    EXPECT_EQ(to_string(-a), "-10000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(to_string(-c), "10000000000000000000000000000000000000000000000000000");
}

TEST(correctness, string_conv) {
    EXPECT_EQ("100", to_string(big_integer("100")));
    EXPECT_EQ("100", to_string(big_integer("0100")));
    EXPECT_EQ("0", to_string(big_integer("0")));
    //EXPECT_EQ("0", to_string(big_integer("-0")));
    EXPECT_EQ("-1000000000000000", to_string(big_integer("-1000000000000000")));

    big_integer lim = std::numeric_limits<int32_t>::max();
    EXPECT_EQ("2147483647", to_string(lim));
    lim++;
    EXPECT_EQ("2147483648", to_string(lim));
    lim = std::numeric_limits<int32_t>::min();
    EXPECT_EQ("-2147483648", to_string(lim));
    lim--;
    EXPECT_EQ("-2147483649", to_string(lim));
}

namespace
{
    template <typename T>
    void test_converting_ctor(T value) {
        using std::to_string;

        big_integer bi = value;
        EXPECT_EQ(to_string(value), to_string(bi));
    }
} // namespace

TEST(correctness, converting_ctor) {
    using std::numeric_limits;

    test_converting_ctor(numeric_limits<short>::min());
    test_converting_ctor(numeric_limits<short>::max());
    test_converting_ctor(numeric_limits<unsigned short>::min());
    test_converting_ctor(numeric_limits<unsigned short>::max());

    test_converting_ctor(numeric_limits<int>::min());
    test_converting_ctor(numeric_limits<int>::max());
    test_converting_ctor(numeric_limits<unsigned>::min());
    test_converting_ctor(numeric_limits<unsigned>::max());

    test_converting_ctor(numeric_limits<long>::min());
    test_converting_ctor(numeric_limits<long>::max());
    //test_converting_ctor(numeric_limits<unsigned long>::min());
    //test_converting_ctor(numeric_limits<unsigned long>::max());

    test_converting_ctor(numeric_limits<long long>::min());
    test_converting_ctor(numeric_limits<long long>::max());
    //test_converting_ctor(numeric_limits<unsigned long long>::min());
    //test_converting_ctor(numeric_limits<unsigned long long>::max());
}

TEST(correctness, converting_ctor2) {
    big_integer a(1);
    big_integer b(1U);
    big_integer c(1L);
    big_integer d(1UL);
    big_integer e(1LL);
    big_integer f(1ULL);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a == d);
    EXPECT_TRUE(a == e);
    EXPECT_TRUE(a == f);
}

TEST(correctness, converting_ctor3) {
    big_integer a(-1);
    big_integer b(-1L);
    big_integer c(-1LL);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
}

TEST(correctness, converting_ctor4) {
    auto num = static_cast<long long>(-1U) << 32;
    big_integer bignum(num);

    EXPECT_EQ(to_string(bignum), std::to_string(num));
}