// big_integer.cpp : Defines the entry point for the application.
//

#include "big_integer.h"

big_integer::big_integer(int value)
{
	m_sign = value == 0 ? 0 : (value < 0 ? -1 : 1);

    auto uval = static_cast<uint32_t>(std::abs(value));
    while (uval > 0)
    {
        m_nums.push_back(uval % BASE);
        uval /= BASE;
    }
}

big_integer::big_integer(const std::string& str)
{
    // "00" "-0" "+0" etc?
    if (str.empty() || str == "0")
        return;

    m_sign = str[0] == '-' ? -1 : 1;



}

void big_integer::normalize()
{
    while (m_nums.size() > 1 && m_nums.back() == 0)
        m_nums.pop_back();
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

big_integer big_integer::operator - ()
{
    m_sign = -m_sign;
	return *this;
}

big_integer::operator bool() const
{
	return m_sign != 0;
}

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
	out << rhs.toString();
	return out;
}
