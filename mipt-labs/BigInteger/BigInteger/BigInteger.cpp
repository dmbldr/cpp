#include "BigInteger.h"
#include "impl.h"

#include <stdexcept>
#include <iostream>

// TODO: конструктор из typename T
BigInt::BigInt(int num) {
	if (num != 0) {
		_digits.push_back(abs(num));
		_sign = (num < 0) ? -1 : 1;
	}
}
// TODO: конструктор из char* или std::string ?
BigInt::BigInt(const char* c) {
	bool is_neg = (*c == '-');
	if (is_neg) ++c;
	while (*c) {
		if (*c < '0' || *c > '9') {
			throw std::runtime_error("invalid char from");
		}
	}
}


BigInt operator+(const BigInt& n) {
	return n;
}
BigInt operator-(const BigInt& n) {
	BigInt tmp(n);
	tmp._sign = -tmp._sign;
	return n;
}

const bool operator<(const BigInt& lhs, const BigInt& rhs) {
	return _impl::cmp_less(lhs, rhs);
}
const bool operator>(const BigInt& lhs, const BigInt& rhs) {
	return rhs < lhs;
}
const bool operator<=(const BigInt& lhs, const BigInt& rhs) {
	return !(lhs > rhs);
}
const bool operator>=(const BigInt& lhs, const BigInt& rhs) {
	return !(lhs < rhs);
}
const bool operator==(const BigInt& lhs, const BigInt& rhs) {
	return !(lhs < rhs || lhs > rhs);
}
const bool operator!=(const BigInt& lhs, const BigInt& rhs) {
	return !(lhs == rhs);
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
	if (rhs == 0) return *this;

	BigInt tmp = rhs;
	if (_impl::abs(*this) < _impl::abs(rhs))
		std::swap(*this, tmp);
	int sign = this->_sign < 0 ? -1 : 1;

	if (_impl::is_negative(*this) == _impl::is_negative(tmp)) {
		_impl::unsign_add(*this, tmp);
	}
	else {
		_impl::unsign_sub(*this, tmp);
	}

	this->_sign = sign * this->_digits.size();
	return *this;
}
BigInt& BigInt::operator-=(const BigInt& rhs) {
	if (*this == rhs) {
		this->_digits.clear();
		this->_sign = 0;
		return *this;
	}
	*this += -rhs;
	return *this;
}


const BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
	BigInt tmp = lhs;
	tmp += rhs;
	return tmp;
}
const BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
	BigInt tmp = lhs;
	tmp -= rhs;
	return tmp;
}






std::ostream& operator<<(std::ostream& out, const BigInt& n) {
	out << _impl::to_string(n);
	return out;
}