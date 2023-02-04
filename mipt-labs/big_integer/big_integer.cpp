#include "big_integer.h"

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

// string = "12345'678901234"
// nums = { 678901234, 12345 }

big_integer::big_integer(const std::string_view& str)
{
    using namespace std::string_literals;

    if (str.empty() || str == "0"s)
        return;

    m_sign = 1;
    long long str_start = 0;
    if (str[0] == '-')
    {
        m_sign = -1;
        str_start = 1;
    }

    m_nums.reserve(str.size() / DIGITS + 1);
    for (auto i = static_cast<long long>(str.size()) - 1; i >= str_start; i -= DIGITS)
    {
        if (i - str_start < DIGITS)
        {
            m_nums.push_back(std::atoi(str.substr(str_start, i).data()));
        }
        else
        {
            m_nums.push_back(std::atoi(str.substr(i - DIGITS, DIGITS).data()));
        }
    }

    normalize();
}

big_integer& big_integer::operator += (const big_integer& rhs)
{
	// +this +rhs = this + rhs
	// -this -rhs = -(this + rhs)

	// +this -rhs 
	//	  this > rhs => this - rhs
	//	  this < rhs => -(rhs - this)
	// 
	// -this +rhs
	//	  this > rhs => -(this - rhs)
	//	  this < rhs => rhs - this

	return *this;
}

// unsign_inplace_add
// unsign_inplace_div

big_integer& big_integer::operator -= (const big_integer& rhs)
{
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

	return *this;
}

big_integer &big_integer::operator*=(const big_integer &rhs)
{
    return *this;
}

big_integer &big_integer::operator/=(const big_integer &rhs)
{
    return *this;
}

big_integer &big_integer::operator%=(const big_integer &rhs)
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

big_integer big_integer::operator-- (int) &
{
	big_integer copy(*this);
	*this -= 1;
	return copy;
}

big_integer big_integer::operator++ (int) &
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

	if (lhs.m_nums.size() != rhs.m_nums.size())
		return lhs.m_nums.size() < rhs.m_nums.size();

	for (size_t i = 0; i < lhs.m_nums.size(); ++i)
	{
		if (lhs.m_nums[i] != rhs.m_nums[i])
			return lhs.m_nums[i] < rhs.m_nums[i];
	}

	return false;
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
