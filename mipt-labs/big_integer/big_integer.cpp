#include "big_integer.h"

#include <cassert>
#include <iomanip>
#include <sstream>

// ============ member functions ============ //

big_integer::big_integer(int64_t value)
{
    if (value == 0) return;

	m_sign = value < 0 ? -1 : 1;

    auto uval = static_cast<uint64_t>(std::llabs(value));
    while (uval > 0)
    {
        m_nums.push_back(uval % BASE);
        uval /= BASE;
    }
}

big_integer::big_integer(std::string str)
{
    using namespace std::string_literals;

    if (str.empty()) throw std::invalid_argument("empty string");

    if (str == "0"s)
        return;

    m_sign = 1;
    if (str[0] == '-')
    {
        m_sign = -1;
        str = str.substr(1);
    }

    m_nums.reserve(str.size() / DIGITS + 1);
    for (auto i = static_cast<long long>(str.size()); i > 0; i -= DIGITS)
    {
        if (i < DIGITS)
        {
            m_nums.push_back(std::stoul(str.substr(0, i)));
        }
        else
        {
            m_nums.push_back(std::stoul(str.substr(i - DIGITS, DIGITS)));
        }
    }

    normalize();
}

// +this +rhs = this + rhs
// -this -rhs = -(this + rhs)

// +this -rhs
//	  this > rhs => this - rhs
//	  this < rhs => -(rhs - this)
//
// -this +rhs
//	  this > rhs => -(this - rhs)
//	  this < rhs => rhs - this
big_integer& big_integer::operator += (const big_integer& rhs)
{
    if (rhs.m_sign == 0) return *this;

    if (m_sign == rhs.m_sign)
    {
        unsign_inplace_add(m_nums, rhs.m_nums);
    }
    else if (rhs.abs_less(*this))
    {
        unsign_inplace_sub(m_nums, rhs.m_nums);
    }
    else
    {
        // TODO: Избавиться от копирования
        big_integer copy = *this;
        *this = rhs;
        unsign_inplace_sub(m_nums, copy.m_nums);
    }

    if (normalize() == 0) m_sign = 0;
	return *this;
}

// +this -rhs => this + rhs
// -this +rhs => -(this + rhs)
//
// +this +rhs
//	  this > rhs => this - rhs
//	  this < rhs => -(rhs - this)
//
// -this -rhs
//	  this > rhs => -(this - rhs)
//	  this < rhs => rhs - this
//
big_integer& big_integer::operator -= (const big_integer& rhs)
{
    if (rhs.m_sign == 0) return *this;

    if (m_sign != rhs.m_sign)
    {
        unsign_inplace_add(m_nums, rhs.m_nums);
    }
    else if (rhs.abs_less(*this))
    {
        unsign_inplace_sub(m_nums, rhs.m_nums);
    }
    else
    {
        // TODO: Избавиться от копирования
        big_integer copy = *this;
        *this = rhs;
        unsign_inplace_sub(m_nums, copy.m_nums);
    }

    if (normalize() == 0) m_sign = 0;
	return *this;
}

big_integer& big_integer::operator*=(const big_integer &rhs)
{
    return easy_mul(rhs);
}

big_integer& big_integer::operator/=(const big_integer &rhs)
{
    return *this;
}

big_integer& big_integer::operator%=(const big_integer &rhs)
{
    return *this;
}

big_integer& big_integer::operator-- ()
{
	return *this -= 1;
}

big_integer& big_integer::operator++ ()
{
	return *this += 1;
}

big_integer big_integer::operator-- (int)
{
	big_integer copy(*this);
	*this -= 1;
	return copy;
}

big_integer big_integer::operator++ (int)
{
	big_integer copy(*this);
	*this += 1;
	return copy;
}

big_integer big_integer::operator - () const
{
    big_integer copy = *this;
    copy.m_sign = -m_sign;
	return copy;
}

big_integer big_integer::operator + () const
{
    return *this;
}

big_integer::operator bool() const
{
	return !m_nums.empty();
}

// ============ private member functions ============ //

std::size_t big_integer::normalize()
{
    while(!m_nums.empty() && m_nums.back() == 0)
    {
        m_nums.pop_back();
    }

    return m_nums.size();
}

void big_integer::expand(std::size_t n)
{
    if (m_nums.size() < n) m_nums.resize(n);
}

bool big_integer::abs_less(const big_integer& rhs) const
{
    if (m_nums.size() != rhs.m_nums.size())
        return m_nums.size() < rhs.m_nums.size();

    for (size_t i = 0; i < m_nums.size(); ++i)
    {
        if (m_nums[i] != rhs.m_nums[i])
            return m_nums[i] < rhs.m_nums[i];
    }

    return false;
}

big_integer& big_integer::easy_mul(const big_integer& rhs)
{
    if (m_sign == 0) return *this;

    if (rhs == 0)
    {
        *this = big_integer();
        return *this;
    }

    mul_by_big(m_nums, rhs.m_nums);

    if (m_sign == rhs.m_sign) m_sign = 1;
    else m_sign = -1;

    normalize();
    return *this;
}

// ============ static private member functions ============ //

big_integer::limb_t big_integer::get_carry(limb_t sum, limb_t a, limb_t b)
{
    return ( (a & b) | ((a | b) & ~sum) ) >> (LIMB_BITS - 1);
}

big_integer::limb_t big_integer::get_borrow(limb_t sub, limb_t a, limb_t b)
{
    return ( (~a & b) | ((~a | b) & sub) ) >> (LIMB_BITS - 1);
}

void big_integer::unsign_inplace_add(limbs_t& lhs, const limbs_t& rhs)
{
    if (lhs.size() < rhs.size()) lhs.resize(rhs.size());

    limb_t carry = 0;
    for (std::size_t i = 0; i < rhs.size(); ++i)
    {
        limb_t sum = lhs[i] + rhs[i] + carry;
        carry = get_carry(sum, lhs[i], rhs[i]);
        lhs[i] = sum;
    }

    for (std::size_t i = rhs.size(); carry > 0 && i < lhs.size(); ++i)
    {
        limb_t sum = lhs[i] + carry;
        carry = get_carry(sum, lhs[i], carry);
        lhs[i] = sum;
    }

    if (carry > 0) lhs.push_back(carry);
}

/// lhs >= rhs
void big_integer::unsign_inplace_sub(limbs_t& lhs, const limbs_t& rhs)
{
    limb_t borrow = 0;
    for (std::size_t i = 0; i < rhs.size(); ++i)
    {
        limb_t sub = lhs[i] - rhs[i] - borrow;
        borrow = get_borrow(sub, lhs[i], rhs[i]);
        lhs[i] = sub;
    }

    for (std::size_t i = rhs.size(); borrow > 0 && i < lhs.size(); ++i)
    {
        limb_t sub = lhs[i] - borrow;
        borrow = get_borrow(sub, lhs[i], borrow);
        lhs[i] = sub;
    }
}

void big_integer::mul_by_limb(limbs_t& res, const limbs_t& lhs, limb_t rhs, std::size_t shift)
{
    if (rhs == 0) return;

    dlimb_t carry = 0;
    for (std::size_t i = 0; i < lhs.size(); ++i)
    {
        carry += static_cast<dlimb_t>(lhs[i]) * rhs;
        res[i + shift] = static_cast<limb_t>(carry);
        carry >>= LIMB_BITS;
    }

    res[lhs.size() + shift] = carry;
}

void big_integer::mul_by_big(limbs_t& res, const limbs_t& rhs)
{
    auto lhs = res;
    res.resize(lhs.size() + rhs.size() + 1);
    for (size_t i = 0; i < rhs.size(); ++i)
    {
        mul_by_limb(res, lhs, rhs[i], i);
    }
}

// ============ not member functions ============ //

big_integer operator + (const big_integer& lhs, const big_integer& rhs)
{
	big_integer copy(lhs);
	copy += rhs;
	return copy;
}

big_integer operator - (const big_integer& lhs, const big_integer& rhs)
{
	big_integer copy(lhs);
	copy -= rhs;
	return copy;
}

big_integer operator * (const big_integer& lhs, const big_integer& rhs)
{
	big_integer copy(lhs);
	copy *= rhs;
	return copy;
}

big_integer operator / (const big_integer& lhs, const big_integer& rhs)
{
	big_integer copy(lhs);
	copy /= rhs;
	return copy;
}

big_integer operator % (const big_integer& lhs, const big_integer& rhs)
{
	big_integer copy(lhs);
	copy %= rhs;
	return copy;
}

bool operator < (const big_integer& lhs, const big_integer& rhs)
{
	if (lhs.m_sign != rhs.m_sign)
		return lhs.m_sign < rhs.m_sign;

    return lhs.abs_less(rhs);
}

bool operator > (const big_integer& lhs, const big_integer& rhs)
{
	return rhs < lhs;
}

bool operator <= (const big_integer& lhs, const big_integer& rhs)
{
	return !(lhs > rhs);
}

bool operator >= (const big_integer& lhs, const big_integer& rhs)
{
	return !(lhs < rhs);
}

bool operator == (const big_integer& lhs, const big_integer& rhs)
{
	return !(lhs < rhs) && !(rhs < lhs);
}

bool operator != (const big_integer& lhs, const big_integer& rhs)
{
	return !(lhs == rhs);
}

//std::istream& operator >> (std::istream& in, big_integer& rhs)
//{
// 
//	return in;
//}

std::ostream& operator << (std::ostream& out, const big_integer& rhs)
{
	if (rhs.m_sign == 0)
    {
        out << 0;
        return out;
    }

    if (rhs.m_sign < 0) out << '-';

    out << rhs.m_nums.back();

    auto prev_fill = out.fill('0');
    for (auto i = static_cast<long long>(rhs.m_nums.size()) - 2; i >= 0; --i)
    {
        out << std::setw(big_integer::DIGITS) << rhs.m_nums[i];
    }
    out.fill(prev_fill);

    return out;
}

std::string to_string(const big_integer& rhs)
{
    std::stringstream ss;
    ss << rhs;
    return ss.str();
}