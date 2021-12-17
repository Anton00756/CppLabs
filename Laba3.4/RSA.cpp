#include "RSA.h"

void RSA_algorithm::generate_keys(const int bits, const test test_name, const double& probability)
{
	LongCount p, q;
	bool (*test) (const LongCount&, int) = Fermat_test;
	int accuracy = 30;
	if (test_name == SoloveyStrassen)
	{
		test = SoloveyStrassen_test;
		accuracy = ceil(-log2(1 - probability));
	}
	else if (test_name == MillerRabin)
	{
		test = MillerRabin_test;
		accuracy = ceil(-log2(1 - probability) / 2);
	}
	while (true)
	{
		try
		{
			if (test(p = LongCount::RandomCount().make(bits), accuracy))
				break;
		}
		catch (...)
		{
			continue;
		}
	}
	while (true)
	{
		try
		{
			if (test(q = LongCount::RandomCount().make(bits), accuracy))
				break;
		}
		catch (...)
		{
			continue;
		}
	}
	n = p.Fourier_prod(q);
	LongCount Euler = (p - LongCount("1")).Fourier_prod(q - LongCount("1"));
	if (LongCount("257") < Euler)
		open_key = LongCount("257");
	else if (LongCount("17") < Euler)
		open_key = LongCount("17");
	else if (LongCount("3") < Euler)
		open_key = LongCount("3");
	else
		throw std::exception("Не удалось создать открытую экспоненту!");
	close_key = ext_gcd(open_key, Euler).first_coef;
	if (close_key < LongCount())
		close_key = close_key + Euler;
}

LongCount RSA_algorithm::encrypting(const LongCount& input)
{
	if ((input < LongCount()) || (n - LongCount("1") < input) || !(gcd(input, n) == LongCount("1")))
		throw std::exception("Некорректное входное число!");
	return input.mod_degree(stoi(convert_to_str(open_key.value)), n);
}

LongCount RSA_algorithm::decoding(const LongCount& input)
{
	return input.mod_degree(stoi(convert_to_str(close_key.value)), n);
}