#include "heaps.h"
#include <chrono>
#include "logger.h"
#include "bank.h"
#include "covid.h"
#include "elevator.h"
#include "policlinic.h"

int main(int argc, char* argv[])
{
	system("chcp 1251>nul");
	
	cout << "\tЛабораторная работа №3.3\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

	/*try
	{
		IntComparer cmp;
		Heap<int, int>* heap = new BinHeap<int, int>(&cmp);
		cout << "\t\tБинарная пирамида:" << endl;
		auto begin = chrono::steady_clock::now();
		for (int i = 1; i <= 10000; i++)
			heap->insert(i, 2 * i);
		cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
		begin = chrono::steady_clock::now();
		for (int i = 1; i <= 10000; i++)
			heap->del_min();
		cout << "Удаление минимального: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl << endl;

		delete heap;
		heap = new BinomialHeap<int, int>(&cmp);
		cout << "\t\tБиномиальная пирамида:" << endl;
		begin = chrono::steady_clock::now();
		for (int i = 1; i <= 10000; i++)
			heap->insert(i, 2 * i);
		cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
		begin = chrono::steady_clock::now();
		for (int i = 1; i <= 10000; i++)
			heap->del_min();
		cout << "Удаление минимального: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl << endl;

		delete heap;
		heap = new FibonacciHeap<int, int>(&cmp);
		cout << "\t\tФибоначчиева пирамида:" << endl;
		begin = chrono::steady_clock::now();
		for (int i = 1; i <= 10000; i++)
			heap->insert(i, 2 * i);
		cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
		begin = chrono::steady_clock::now();
		for (int i = 1; i <= 10000; i++)
			heap->del_min();
		cout << "Удаление минимального: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
		delete heap;
	}
	catch (HeapInsertExc error)
	{
		cout << "Вставка: " << error.text() << endl;
	}
	catch (HeapGetMinExc error)
	{
		cout << "Получение: " << error.text() << endl;
	}
	catch (HeapDelMinExc error)
	{
		cout << "Удаление: " << error.text() << endl;
	}


	Logger* log = Logger::Builder().add_stream(Trace, "stdout").build("input.json");	
	log->write(Trace, "check_1");
	log->write(Fatal, "check_2");*/

	/*try
	{
		srand(time(NULL));
		Bank my_bank;
		my_bank.opening();
	}
	catch (std::exception error)
	{
		cout << "Error: " << error.what() << endl;
	}*/

	/*try
	{
		srand(time(NULL));
		Epidemic::Population popul = Epidemic::Population("covid_in.txt", "0.3 0.8 50 true");
		Epidemic covid(popul.get_file());
		covid.start();
		cout << "Simulation completed!" << endl;
	}
	catch (std::exception error)
	{
		cout << "Error: " << error.what() << endl;
	}*/

	/*string properties(argv[1]), passengers(argv[2]);

	try
	{
		LiftSystem house(properties, passengers);
		house.start();
	}
	catch (const std::exception& error)
	{
		cout << "Error: " << error.what() << endl;
	}*/

	try
	{
		srand(time(NULL));
		HealthCenter center(5, 3, 10);
		center.start();
	}
	catch (const std::exception& error)
	{
		cout << "Error: " << error.what() << endl;
	}

	system("pause>nul");
}