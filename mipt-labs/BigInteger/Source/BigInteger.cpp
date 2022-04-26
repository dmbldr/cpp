//
// Created by serBo on 24.04.2022.
//

#include <BigInteger.hpp>

big_integer::big_integer(uint64_t num)
{
    while(num > 0)
    {
        limbs.push_back(num % base);
        num /= base;
    }
    // TODO: signed
}

big_integer& big_integer::operator += (const big_integer &rhs)
{
    size_t i = 0;
    uint64_t carry = 0;
    while(i < this->limbs.size() || i < rhs.limbs.size())
    {
        uint64_t sum = this->limbs[i] + rhs.limbs[i] + carry;
        carry = sum > base ? 1 : 0;
        this->limbs[i] = sum % base;
        ++i;
    }
    while(i < rhs.limbs.size()) // случай this.size() < rhs.size()
    {
        uint64_t sum = rhs.limbs[i] + carry;
        carry = sum > base ? 1 : 0;
        this->limbs.push_back(sum % base);
        ++i;
    }
    while(i < this->limbs.size()) // случай this.size() > rhs.size()
    {
        uint64_t sum = this->limbs[i] + carry;
        carry = sum > base ? 1 : 0;
        this->limbs[i] = sum % base;
        ++i;
    }
    // TODO: убрать дублирование кода
    return *this;
}

big_integer& big_integer::operator -= (const big_integer &rhs)
{
    // TODO
    return *this;
}

big_integer& big_integer::operator *= (const big_integer &rhs)
{
    // TODO
    return *this;
}

big_integer& big_integer::operator /= (const big_integer &rhs)
{
    // TODO
    return *this;
}

big_integer& big_integer::operator %= (const big_integer &rhs)
{
    // TODO
    return *this;
}

bool operator < (const big_integer &lhs, const big_integer &rhs)
{
    if(lhs.limbs.size() != rhs.limbs.size())
    {
        return lhs.limbs.size() < rhs.limbs.size();
    }
    auto i = lhs.limbs.size();
    while(--i > 0 && lhs.limbs[i] == rhs.limbs[i])
        ;
    return lhs.limbs[i] < rhs.limbs[i];
}

bool operator > (const big_integer &lhs, const big_integer &rhs)
{
    return rhs < lhs;
}

bool operator == (const big_integer &lhs, const big_integer &rhs)
{
    return !(lhs < rhs) && !(rhs < lhs);
}

bool operator != (const big_integer &lhs, const big_integer &rhs)
{
    return !(lhs == rhs);
}

bool operator <= (const big_integer &lhs, const big_integer &rhs)
{
    return !(lhs > rhs);
}

bool operator >= (const big_integer& lhs, const big_integer& rhs)
{
    return !(lhs < rhs);
}

big_integer operator + (const big_integer& lhs, const big_integer& rhs)
{
    big_integer copy = lhs;
    copy += rhs;
    return copy;
}

big_integer operator - (const big_integer& lhs, const big_integer& rhs)
{
    big_integer copy = lhs;
    copy -= rhs;
    return copy;
}

big_integer operator * (const big_integer& lhs, const big_integer& rhs)
{
    big_integer copy = lhs;
    copy *= rhs;
    return copy;
}

big_integer operator / (const big_integer& lhs, const big_integer& rhs)
{
    big_integer copy = lhs;
    copy /= rhs;
    return copy;
}

big_integer operator % (const big_integer& lhs, const big_integer& rhs)
{
    big_integer copy = lhs;
    copy %= rhs;
    return copy;
}

size_t big_integer::normalize()
{
    while (!limbs.empty() && limbs.back() == 0)
    {
        limbs.pop_back();
    }
    return limbs.size();
}