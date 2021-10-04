#include "tasks.h"

int main(int argc, char* argv[])
{
    system("chcp 1251>nul");

    string str;

    cout << "\tЛабораторная работа №3.1\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

    while (true)
    {
        cout << "1. Поиск строки в файлах\n2. Проверка выпуклости\n3. Значение многочлена\n4. Минимальная СС\n5. Сложение чисел в заданной СС\n6. Интерпретатор\n\n";
        cout << "Выберите пункт: ";
        getline(cin, str);
        cout << endl;
        try
        {
            if (str.length() != 1)
                throw 666;
            switch (str[0])
            {
            case '1':
                task_1();
                break;
            case '2':
                task_2();
                break;
            case '3':
                task_3();
                break;
            case '4':
                task_4(argv[1]); // C:\Users\anton\source\repos\Laba3.1\Laba3.1\in4.txt
                break;
            case '5':
                task_5();
                break;
            case '6':
                if ((argc == 4) && !strcmp(argv[2], "/trace"))
                {
                    ofstream out(argv[3]);
                    if (!out.is_open())
                        cout << "Ошибка открытия файла вывода";
                    else
                    {
                        task_6(argv[1], out, true);
                        out.close();
                    }
                }
                else
                    task_6(argv[1], cout, true); // C:\Users\anton\source\repos\Laba3.1\Laba3.1\in6.txt
                break;
            default:
                throw 666;
                break;
            }
        }
        catch (...)
        {
            cout << "Некорректный ввод!";
        }
        cout << endl << endl;
    }

    system("pause>nul");
}