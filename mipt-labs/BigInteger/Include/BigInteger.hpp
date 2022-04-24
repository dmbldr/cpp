#ifndef BIGINTEGER_BIGINTEGER_HPP
#define BIGINTEGER_BIGINTEGER_HPP

#include <string>
#include <iostream>
#include <vector>

class big_integer
{
public:
    big_integer() = default;
    big_integer(int num);
    explicit big_integer(const std::string& str);

    big_integer(const big_integer& other);
    big_integer& operator=(const big_integer& other);


    big_integer& operator+=(const big_integer& rhs);
    big_integer& operator-=(const big_integer& rhs);
    big_integer& operator*=(const big_integer& rhs);
    big_integer& operator/=(const big_integer& rhs);
    big_integer& operator%=(const big_integer& rhs);

    big_integer operator-() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator < (const big_integer& lhs, const big_integer& rhs);

    friend std::istream& operator>>(std::istream& in, big_integer& value);
    friend std::string to_string(const big_integer& value);

    operator bool();

private:
    const uint64_t base = 10;

    std::vector<int64_t> value;
};

bool operator == (const big_integer& lhs, const big_integer& rhs);
bool operator != (const big_integer& lhs, const big_integer& rhs);
bool operator <= (const big_integer& lhs, const big_integer& rhs);
bool operator >= (const big_integer& lhs, const big_integer& rhs);
bool operator < (const big_integer& lhs, const big_integer& rhs);
bool operator > (const big_integer& lhs, const big_integer& rhs);

big_integer operator + (const big_integer& lhs, const big_integer& rhs);
big_integer operator - (const big_integer& lhs, const big_integer& rhs);
big_integer operator * (const big_integer& lhs, const big_integer& rhs);
big_integer operator / (const big_integer& lhs, const big_integer& rhs);
big_integer operator % (const big_integer& lhs, const big_integer& rhs);

std::istream& operator>>(std::istream& in, big_integer& value);
std::ostream& operator<<(std::ostream& out, const big_integer& value);

#endif //BIGINTEGER_BIGINTEGER_HPP
