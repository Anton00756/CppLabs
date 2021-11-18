#include "trees.h"
#include <chrono>

int main()
{
    system("chcp 1251>nul");

    cout << "\tЛабораторная работа №3.2\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

	IntComparer int_cmp;
	AVL<int, string> tree(&int_cmp);
	tree.insert(1, "Один");
	tree.insert(2, "Два");
	tree.insert(3, "Три");
	cout << "Под номером 2: " << tree.find(2) << endl;
	tree.remove(2);
	try
	{
		cout << "Под номером 2: " << tree.find(2) << endl;
		//tree.remove(2);
	}
	catch (const BST<int, string>::Find_Err& er)
	{
		cout << "Поисковик: " << er.what() << endl;
	}
	catch (const BST<int, string>::Tree_Ex& er)
	{
		cout << er.what() << endl;
	}

	AVL<int, int> tree_2(&int_cmp);
	RB<int, int> tree_3(&int_cmp);
	Splay<int, int> tree_4(&int_cmp);
	T_23<int, int> tree_5(&int_cmp);

	cout << endl << "\t\tАВЛ-дерево:" << endl;
	auto begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_2.insert(i, 2 * i);
	cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_2.find(i);
	cout << "Поиск: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_2.remove(i);
	cout << "Удаление: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	
	cout << endl << "\t\tКЧ-дерево:" << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_3.insert(i, 2 * i);
	cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_3.find(i);
	cout << "Поиск: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_3.remove(i);
	cout << "Удаление: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;

	cout << endl << "\t\tКосое дерево:" << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_4.insert(i, 2 * i);
	cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_4.find(i);
	cout << "Поиск: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_4.remove(i);
	cout << "Удаление: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;

	cout << endl << "\t\t2-3 дерево:" << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_5.insert(i, 2 * i);
	cout << "Вставка: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_5.find(i);
	cout << "Поиск: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;
	begin = chrono::steady_clock::now();
	for (int i = 1; i <= 10000; i++)
		tree_5.remove(i);
	cout << "Удаление: " << (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin)).count() << endl;

    system("pause>nul");
}
