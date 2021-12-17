#ifndef RSA
#define RSA
#include "LongCount.h"

class RSA_algorithm
{
public:
	RSA_algorithm() {}
	~RSA_algorithm() {}

	LongCount open_key, n;

	enum test { Fermat, SoloveyStrassen, MillerRabin };
	void generate_keys(const int bits, const test test_name, const double& probability);
	LongCount encrypting(const LongCount& input);
	LongCount decoding(const LongCount& input);

public:
	LongCount close_key;
};
#endif