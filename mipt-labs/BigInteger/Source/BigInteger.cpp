//
// Created by serBo on 24.04.2022.
//

#include <BigInteger.hpp>

big_integer::big_integer() : sign(false)
{
    limbs.emplace_back(0);
}

big_integer::big_integer(int num)
{
    limbs.push_back(std::abs(num));
    sign = num < 0;
}

big_integer& big_integer::operator += (const big_integer &rhs)
{
    expand(rhs.limbs.size());
    auto carry = unsign_add_inplace(this->limbs, rhs.limbs);
    if(carry) limbs.emplace_back(1);
    return *this;
}

big_integer& big_integer::operator -= (const big_integer &rhs)
{


    // TODO
    normalize();
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
    if (lhs.sign != rhs.sign)
    {
        return lhs.sign;
    }
    return lhs.sign ^ big_integer::less(lhs.limbs, rhs.limbs);
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




void big_integer::expand(size_t n)
{
    if(limbs.size() < n)
    {
        limbs.resize(n);
    }
}

size_t big_integer::normalize()
{
    while (!limbs.empty() && limbs.back() == 0)
    {
        limbs.pop_back();
    }
    return limbs.size();
}

bool big_integer::less(const std::vector<limb_t> &lhs, const std::vector<limb_t> &rhs)
{
    if(lhs.size() != rhs.size())
    {
        return lhs.size() < rhs.size();
    }
    auto i = lhs.size();
    while(--i > 0 && lhs[i] == rhs[i])
        ;
    return lhs[i] < rhs[i];
}

big_integer::limb_t
big_integer::unsign_add_inplace(std::vector<limb_t> &lhs, const std::vector<limb_t>& rhs)
{
    limb_t carry = 0;
    for (size_t i = 0; i < rhs.size(); ++i)
    {
        limb_t sum = lhs[i] + rhs[i] + carry;
        carry = 0;
        lhs[i] = sum;
    }
    // случай this.size() > rhs.size()
    for (size_t i = rhs.size(); i < lhs.size(); ++i)
    {
        limb_t sum = lhs[i] + carry;
        carry = 0;
        lhs[i] = sum;
    }

    return carry;
}




