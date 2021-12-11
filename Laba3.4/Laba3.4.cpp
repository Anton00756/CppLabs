#include "LongCount.h"
#include <chrono>

int main()
{
	system("chcp 1251>nul");
	srand(static_cast<unsigned int>(time(0)));

	cout << "\tЛабораторная работа №3.4\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

	LongCount A("4294967299", 10), B = A, C;
	C = A;
	cout << A * LongCount("2") << endl;
	A = LongCount("123456789123456789123456789123456789");
	B = LongCount("CC", 16);
	cout << A << "\t" << B << "\t" << C << endl;
	LongCount D = A - (A + LongCount("11"));
	cout << "A + 1 = " << A + LongCount("1", 10) << "\tA - B = " << A - B << endl;
	cout << "D = " << D << "\t" << endl << LongCount("412412411") * LongCount("1231928312481") << endl;
	cout << LongCount("99") * LongCount("1000000000000000") << endl;
	cout << LongCount("2") * LongCount("7") << endl;
	cout << "Рандомное число из 1024 бит: " << LongCount(LongCount::RandomCount().make(1024)) << endl;
	
	system("pause>nul");
}