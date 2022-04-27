#ifndef BIGINTEGER_BIGINTEGER_HPP
#define BIGINTEGER_BIGINTEGER_HPP

#include <string>
#include <iostream>
#include <vector>

class big_integer
{
public:
    big_integer();
    big_integer(int num);
    explicit big_integer(const std::string& str);

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
    using limb_t = uint32_t;

    static constexpr size_t LIMB_BITS = sizeof(limb_t) * 8;

    /// В конце вектора находятся старшие лимбы (коэффициенты)
    std::vector<limb_t> limbs;

    /// true - отрицательно
    bool sign;
private:

    void expand(size_t n);
    size_t normalize();

    static bool less(const std::vector<limb_t>& lhs, const std::vector<limb_t>& rhs);

    static limb_t unsign_add_inplace(std::vector<limb_t>& lhs, const std::vector<limb_t>& rhs);
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
