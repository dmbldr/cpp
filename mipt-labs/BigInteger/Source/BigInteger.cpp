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

size_t big_integer::normalize()
{
    while (!limbs.empty() && limbs.back() == 0)
    {
        limbs.pop_back();
    }
    return limbs.size();
}