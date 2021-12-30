#pragma once

#include "BigInteger.h"

struct _impl {
	static void normalize(BigInt& n);
	static std::string to_string(const BigInt& n);

	static BigInt abs(const BigInt& n);
	static bool is_negative(const BigInt& n);

	/* ��������� �� ������� ���� �������� ������� �����
	*/
	static bool cmp_less(const BigInt& lhs, const BigInt& rhs);

	/* �������� ���� ������� ����� ��� ����� �����
		��������� ����� ������� � lhs
	*/
	static void unsign_add(BigInt& lhs, const BigInt& rhs);
	/* ��������� ���� ������� ����� ��� ����� �����
		����������� lhs > rhs 
		��������� ����� ������� � lhs
	*/
	static void unsign_sub(BigInt& lhs, const BigInt& rhs);
};

