#pragma once

#include "BigInteger.h"

struct _impl {
	static void normalize(BigInt& n);
	static std::string to_string(const BigInt& n);

	static BigInt abs(const BigInt& n);
	static bool is_negative(const BigInt& n);

	/* Сравнение на меньшее двух знаковых больших числа
	*/
	static bool cmp_less(const BigInt& lhs, const BigInt& rhs);

	/* Сложение двух больших чисел без учёта знака
		Результат сразу пишется в lhs
	*/
	static void unsign_add(BigInt& lhs, const BigInt& rhs);
	/* Вычитание двух больших чисел без учёта знака
		Обязательно lhs > rhs 
		Результат сразу пишется в lhs
	*/
	static void unsign_sub(BigInt& lhs, const BigInt& rhs);
};

