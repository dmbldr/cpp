//
// Created by serBo on 24.04.2022.
//

#include <BigInteger.hpp>

class big_integer::Implement
{
public:
    Implement() = default;

    size_t normalize()
    {
        while (!limbs.empty() && limbs.back() == 0)
        {
            limbs.pop_back();
        }
        return limbs.size();
    }

public:
    /// основание - 2^32
    const uint32_t base = 0xFFFFFFFF;

    /// В конце вектора находятся старшие лимбы (коэффициенты)
    std::vector<uint32_t> limbs;
};

big_integer::big_integer(uint32_t num) : pimpl(std::make_unique<Implement>())
{
    pimpl->limbs.push_back(num);
    // TODO: signed
}

big_integer& big_integer::operator += (const big_integer &rhs)
{
    size_t i = 0;
    uint64_t carry = 0;
    while(i < this->pimpl->limbs.size() || i < rhs.pimpl->limbs.size())
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
    if(lhs.pimpl->limbs.size() != rhs.pimpl->limbs.size())
    {
        return lhs.pimpl->limbs.size() < rhs.pimpl->limbs.size();
    }
    auto i = lhs.pimpl->limbs.size();
    while(--i > 0 && lhs.pimpl->limbs[i] == rhs.pimpl->limbs[i])
        ;
    return lhs.pimpl->limbs[i] < rhs.pimpl->limbs[i];
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