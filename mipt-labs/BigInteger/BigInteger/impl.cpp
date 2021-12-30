#include "BigInteger.h"
#include "impl.h"

#include <limits>
#include <algorithm>

void _impl::normalize(BigInt& n) {
	size_t sign = std::abs(n._sign);
	while (sign > 0 && n._digits[sign - 1] == 0) {
		n._digits.pop_back();
		--sign;
	}
	if (n._sign < 0) n._sign = -sign;
	else n._sign = sign;
}

// TODO: выводит "лимбы" поочерёдно от менее значащих
std::string _impl::to_string(const BigInt& n) {
	std::string res;
	if (n._sign == 0) return "0";
	for (int i = n._digits.size() - 1; i >= 0; --i) {
		res += std::to_string(n._digits[i]);
	}
	if (n._sign < 0) res.insert(res.cbegin(), '-');
	return res;
}

BigInt _impl::abs(const BigInt& n) {
	BigInt res = n;
	res._sign = std::abs(res._sign);
	return res;
}

bool _impl::is_negative(const BigInt& n) {
	return n._sign < 0;
}

bool _impl::cmp_less(const BigInt& lhs, const BigInt& rhs) {
	if (lhs._sign != rhs._sign) 
		return lhs._sign < rhs._sign;
	else {
		for (int i = lhs._digits.size()-1; i >= 0; --i) {
			if (lhs._digits[i] != rhs._digits[i]) {
				bool less = lhs._digits[i] < rhs._digits[i];
				return lhs._sign > 0 ? less : !less; 
			}
		}
		return false;
	}
}

void _impl::unsign_add(BigInt& lhs, const BigInt& rhs) {
	if (lhs._digits.size() < rhs._digits.size())
		for (size_t i = 0; i < rhs._digits.size() - lhs._digits.size(); ++i)
			lhs._digits.push_back(0);

	BigInt::base_type carry = 0;
	for (size_t i = 0; i < rhs._digits.size(); ++i) {
		BigInt::base_type sum = lhs._digits[i] + rhs._digits[i] + carry;
		carry = ((lhs._digits[i] & rhs._digits[i]) | ((lhs._digits[i] | rhs._digits[i]) & ~sum)) >> 31;
		lhs._digits[i] = sum;
	}

	if (carry) {
		lhs._digits.push_back(carry);
	}
	lhs._sign = lhs._digits.size();
}

void _impl::unsign_sub(BigInt& lhs, const BigInt& rhs) {
	BigInt::base_type borrow = 0;
	for (size_t i = 0; i < rhs._digits.size(); ++i) {
		BigInt::base_type diff = lhs._digits[i] - rhs._digits[i] - borrow;
		borrow = ((~lhs._digits[i] & rhs._digits[i]) | ((~lhs._digits[i] | rhs._digits[i]) & diff)) >> 31;
		lhs._digits[i] = diff;
	}

	for (size_t i = rhs._digits.size(); i < lhs._digits.size(); ++i) {
		BigInt::base_type diff = lhs._digits[i] - borrow;
		borrow = ((~lhs._digits[i] & borrow) | ((~lhs._digits[i] | borrow) & diff)) >> 31;
		lhs._digits[i] = diff;
		if (borrow) break;
	}
	normalize(lhs);
}