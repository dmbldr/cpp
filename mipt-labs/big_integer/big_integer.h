#pragma once

#include <iostream>
#include <vector>
#include <string>

class big_integer
{
public:
	big_integer() = default;
	big_integer(int);
    explicit big_integer(const std::string&);

	big_integer& operator += (const big_integer& rhs);
	big_integer& operator -= (const big_integer& rhs);
	big_integer& operator *= (const big_integer& rhs);
	big_integer& operator /= (const big_integer& rhs);
	big_integer& operator %= (const big_integer& rhs);

	big_integer& operator-- ();
	big_integer& operator++ ();

	big_integer operator-- (int) &;
	big_integer operator++ (int) &;

    big_integer operator+() const;
	big_integer operator-() const;

	friend bool operator < (const big_integer& lhs, const big_integer& rhs);

	friend std::istream& operator >> (std::istream& in, big_integer& rhs);
	
	friend std::string to_string();

	explicit operator bool() const;

private:

    /// Храним по 9 цифр в одном элементе
	static constexpr uint32_t BASE = 1'000'000'000UL;

    //!  0: BigInt == 0;
    //!  1: BigInt > 0;
    //! -1: BigInt < 0;
	int m_sign = 0;

    ///
	std::vector<uint32_t> m_nums;
};


bool operator < (const big_integer& lhs, const big_integer& rhs);
bool operator > (const big_integer& lhs, const big_integer& rhs);
bool operator <= (const big_integer& lhs, const big_integer& rhs);
bool operator >= (const big_integer& lhs, const big_integer& rhs);
bool operator == (const big_integer& lhs, const big_integer& rhs);
bool operator != (const big_integer& lhs, const big_integer& rhs);

big_integer operator + (const big_integer& lhs, const big_integer& rhs);
big_integer operator - (const big_integer& lhs, const big_integer& rhs);
big_integer operator * (const big_integer& lhs, const big_integer& rhs);
big_integer operator / (const big_integer& lhs, const big_integer& rhs);
big_integer operator % (const big_integer& lhs, const big_integer& rhs);

std::istream& operator >> (std::istream& in, big_integer& rhs);
std::ostream& operator << (std::ostream& out, const big_integer& rhs);

std::string to_string(const big_integer& rhs);