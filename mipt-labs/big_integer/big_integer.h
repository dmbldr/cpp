#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class big_integer
{
public:
	big_integer() = default;
	big_integer(int64_t);
    explicit big_integer(std::string);

	big_integer& operator += (const big_integer& rhs);
	big_integer& operator -= (const big_integer& rhs);
	big_integer& operator *= (const big_integer& rhs);
	big_integer& operator /= (const big_integer& rhs);
	big_integer& operator %= (const big_integer& rhs);

	big_integer& operator-- ();
	big_integer& operator++ ();

	big_integer operator-- (int);
	big_integer operator++ (int);

    big_integer operator+() const;
	big_integer operator-() const;

	friend bool operator < (const big_integer& lhs, const big_integer& rhs);

	friend std::istream& operator >> (std::istream& in, big_integer& rhs);
    friend std::ostream& operator << (std::ostream& out, const big_integer& rhs);

	explicit operator bool() const;

private:

    /// Храним по 9 цифр в одном элементе
	static constexpr uint64_t BASE = 1'000'000'000UL;
    static constexpr int DIGITS = 9;

    ///  0: BigInt == 0;
    ///  1: BigInt >  0;
    /// -1: BigInt <  0;
	int m_sign = 0;

    /// Основание СС = 2^32
    static constexpr uint32_t LIMB_BITS = 32;
    using limb_t = uint32_t;
    using limbs_t = std::vector<limb_t>;

    limbs_t m_nums;

private:
    std::size_t normalize();
    void expand(std::size_t);

    bool abs_less(const big_integer& rhs) const;
private:
    static limb_t get_carry(limb_t sum, limb_t a, limb_t b);
    static limb_t get_borrow(limb_t sub, limb_t a, limb_t b);
    static void unsign_inplace_add(limbs_t& lhs, const limbs_t& rhs);
    static void unsign_inplace_sub(limbs_t& lhs, const limbs_t& rhs);
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