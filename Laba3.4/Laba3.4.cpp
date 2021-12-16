#include "LongCount.h"
#include <chrono>

int main()
{
	system("chcp 1251>nul");
	srand(static_cast<unsigned int>(time(0)));

	cout << "\tЛабораторная работа №3.4\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

	try
	{
		LongCount A("4294967299", 10), B = A, C;
		C = A;
		cout << A << " * 2 = " << A * LongCount("2") << endl;
		A = LongCount("123456789123456789123456789123456789");
		B = LongCount("CC", 16);
		cout << "A = " << A << "\tB = " << B << "\tC = " << C << endl;
		LongCount D = A - (A + LongCount("11"));
		cout << "A + 1 = " << A + LongCount("1", 10) << "\tA - B = " << A - B << endl;
		cout << "D = " << D << "\t" << endl << endl << "41241241133765 * -1231928312481 = " << LongCount("41241241133765") * LongCount("-1231928312481") << endl;
		cout << "Karatsuba:\n-41241241133765 * 1231928312481 = " << LongCount("-41241241133765").karatsuba(LongCount("1231928312481")) << endl << endl;
		cout << "Рандомное число из 1024 бит: " << LongCount(LongCount::RandomCount().make(1024)) << endl;

		cout << endl << "95643841241241133785091112846127841264128741248912748912741274987124 * 149871429812741298471241274877291231928312441231928312449123129389123192831244123192831244912312938743291231928312441231928312449123129389123192831244123192831244912312938743 = " << LongCount("95643841241241133785091112846127841264128741248912748912741274987124") * (LongCount("149871429812741298471241274877291231928312441231928312449123129389123192831244123192831244912312938743291231928312441231928312449123129389123192831244123192831244912312938743")) << endl;
		cout << endl << "Karatsuba:\n95643841241241133785091112846127841264128741248912748912741274987124 * 149871429812741298471241274877291231928312441231928312449123129389123192831244123192831244912312938743291231928312441231928312449123129389123192831244123192831244912312938743 = " << LongCount("95643841241241133785091112846127841264128741248912748912741274987124").karatsuba(LongCount("149871429812741298471241274877291231928312441231928312449123129389123192831244123192831244912312938743291231928312441231928312449123129389123192831244123192831244912312938743")) << endl;
		cout << endl << "Fourier:\n95643841241241133785091112846127841264128741248912748912741274987124 * 149871429812741298471241274877291231928312441231928312449123129389123192831244123192831244912312938743291231928312441231928312449123129389123192831244123192831244912312938743 = " << LongCount("95643841241241133785091112846127841264128741248912748912741274987124").Fourier_prod(LongCount("149871429812741298471241274877291231928312441231928312449123129389123192831244123192831244912312938743291231928312441231928312449123129389123192831244123192831244912312938743")) << endl;

		cout << endl << "-1398947812412412412 в 33 степени: " << LongCount("-1398947812412412412").degree(33) << endl;
		cout << endl << "4 в 13 степени по модулю 497: " << LongCount("4").mod_degree(13, LongCount("497")) << endl;

		cout << "26 / 12 = " << LongCount("26") / LongCount("12") << endl;
		cout << endl << "gcd(616, 364) = " << gcd(LongCount("616"), LongCount("364")) << endl;
		LC_struct result = ext_gcd(LongCount("616"), LongCount("364"));
		cout << "ext_gcd(616, 364) = " << result.first_coef << " * f + " << result.second_coef << " * g = " << result.gcd << endl;
		result = ext_gcd(LongCount("51"), LongCount("3"));
		cout << "ext_gcd(51, 3) = " << result.first_coef << " * f + " << result.second_coef << " * g = " << result.gcd << endl;
		result = ext_gcd(LongCount("5"), LongCount("2"));
		cout << "ext_gcd(5, 2) = " << result.first_coef << " * f + " << result.second_coef << " * g = " << result.gcd << endl;
		cout << "bin_gcd(616, 364) = " << bin_gcd(LongCount("616"), LongCount("364")) << endl;

		cout << endl;
		for (LongCount i = LongCount("1"); i < LongCount("31"); i = i + LongCount("1"))
			cout << "Символ Лежандра для (" << i << ", 29): " << Legendre_symbol(i, LongCount("29")) << endl;
		cout << "Символ Якоби для (2, 15): " << Jacobi_symbol(LongCount("2"), LongCount("15")) << endl << endl;

		cout << "Тест Ферма для числа 97: " << (Fermat_test(LongCount("97"), 5) ? "простое." : "непростое.") << endl;
		if (SoloveyStrassen_test(LongCount("97"), 5))
			cout << "Тест Соловея-Штрассена для числа 97: простое с вероятностью " << SoloveyStrassen_probability(5) << "%." << endl;
		else
			cout << "Тест Соловея-Штрассена для числа 97: непростое." << endl;
		if (MillerRabin_test(LongCount("97"), 5))
			cout << "Тест Миллера-Рабина для числа 97: простое с вероятностью " << MillerRabin_probability(5) << "%." << endl;
		else
			cout << "Тест Миллера-Рабина для числа 97: непростое." << endl;
	}
	catch (std::exception error)
	{
		cout << "Error: " << error.what() << endl;
	}

	system("pause>nul");
	return 0;
}