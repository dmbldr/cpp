#pragma once

#include <vector>
#include <string>

class BigInt {
public:
	using base_type = uint32_t;

	BigInt() = default;
	BigInt(int num);
	BigInt(const char* c);

	// compare
	friend const bool operator<(const BigInt& lhs, const BigInt& rhs);
	friend const bool operator>(const BigInt& lhs, const BigInt& rhs);
	friend const bool operator<=(const BigInt& lhs, const BigInt& rhs);
	friend const bool operator>=(const BigInt& lhs, const BigInt& rhs);

	friend const bool operator==(const BigInt& lhs, const BigInt& rhs);
	friend const bool operator!=(const BigInt& lhs, const BigInt& rhs);

	// unary
	friend BigInt operator+(const BigInt& n);
	friend BigInt operator-(const BigInt& n);

	// arithmetic asigment
	/*	Знаковое сложение */
	BigInt& operator+=(const BigInt& rhs);
	/*	Знаковое вычитание */
	BigInt& operator-=(const BigInt& rhs);


	// binary arithmetic
	friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);

	

	// ioput
	friend std::ostream& operator<<(std::ostream& out, const BigInt& n);


private:
	/* Определяет размер в лимбах и знак числа
		Если _sign > 0, то число положительное
		Если _sign < 0, то число отрицательное
		Если _sign = 0, то число равно нулю
	*/
	long _sign = 0;
	/* Массив лимбов
		Лимб - коэффициент при основании СС
	*/
	std::vector<base_type> _digits;

	friend struct _impl;
};

#include "impl.h"