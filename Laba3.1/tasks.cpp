#include "tasks.h"

// Задание №1
int find_substr(const string& filename, const string& substr)
{
    ifstream in(filename);
    if (!in.is_open())
        cout << "Ошибка открытия файла!";
    else
    {
        int pos = 0;
        try
        {
            while (!in.eof())
            {
                if (in.peek() != substr[0])
                    in.get();
                else
                {
                    pos = in.tellg();
                    try
                    {
                        for (auto symbol : substr)
                        {
                            if ((in.peek() == symbol) || ((in.peek() == '\n') && (symbol == ' ')))
                                in.get();
                            else
                                throw 666;
                        }
                        throw 777;
                    }
                    catch (int num)
                    {
                        if (num == 777)
                            throw;
                    }
                    in.seekg(pos + 1);
                }
            }
        }
        catch (...)
        {
            in.close();
            return pos;
        }
        in.close();
    }
    return -1;
}

string searching(int (*fPtr)(const string&, const string&), const string& substr, int count, ...)
{
    va_list string_list;
    va_start(string_list, count);
    string result = "", filename;
    int number;
    for (int i = 1; i <= count; i++)
    {
        filename = va_arg(string_list, string);
        number = fPtr(filename, substr);
        if (number != -1)
            result += "В файле '" + filename + "' найдена искомая строка. Позиция: " + to_string(number) + ".\n";
    }
    if (result == "")
        result = "В переданных файлах заданная подстрока не содержится!";
    else
        result = result.substr(0, result.length() - 1);
    va_end(string_list);
    return result;
}

void task_1()
{
    string name1 = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in1_1.txt";
    string name2 = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in1_2.txt";
    string name3 = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in1_3.txt";
    cout << searching(find_substr, "hel lo", 3, name1, name2, name3);
}

// Задание №2
struct Point
{
    int x, y;
} typedef point;

bool camber_check(int count, ...)
{
    if (count < 3)
        return false;
    if (count == 3)
        return true;
    va_list args;
    va_start(args, count);
    point* arg, * top, * first = va_arg(args, point*), * second = va_arg(args, point*);
    top = first;
    point a, b;
    bool camber = true, plus;
    for (int i = 0; i < count - 2; i++)
    {
        arg = va_arg(args, point*);

        a = { second->x - first->x , second->y - first->y };
        b = { arg->x - second->x , arg->y - second->y };

        if (!i)
            plus = (a.x * b.y - a.y * b.x > 0);
        else
        {
            if ((a.x * b.y - a.y * b.x > 0) != plus)
            {
                camber = false;
                break;
            }
            else
                plus = (a.x * b.y - a.y * b.x > 0);

            if (i == count - 3)
            {
                a = { arg->x - second->x , arg->y - second->y };
                b = { top->x - arg->x , top->y - arg->y };
                plus = (a.x * b.y - a.y * b.x > 0);

                if ((a.x * b.y - a.y * b.x > 0) != plus)
                {
                    camber = false;
                    break;
                }
            }
        }

        first = second;
        second = arg;
    }
    va_end(args);
    return camber;
}

void task_2()
{
    point p1 = { 0, 0 }, p2 = { 0, 10 }, p3 = { 10, 10 }, p4 = { 10, 0 };
    //point p1 = { 0, 0 }, p2 = { 5, 5 }, p3 = { 0, 10 }, p4 = { 10, 5 };
    if (camber_check(4, &p1, &p2, &p3, &p4))
        cout << "Многоугольник выпуклый.";
    else
        cout << "Многоугольник невыпуклый.";
}

// Задание №3
double decision(double x, int level, ...)
{
    if (level < 0)
        return 0;
    va_list coef_list;
    va_start(coef_list, level);
    double result = va_arg(coef_list, double);
    if (level)
    {
        for (int i = 1; i <= level; i++)
            result = result * x + va_arg(coef_list, double);
    }
    va_end(coef_list);
    return result;
}

void task_3()
{
    cout << "Значение многочлена 2*x^3 - 7*x^2 + 5*x + 20 в точке x = 3: " << decision(3.0, 3, 2.0, -7.0, 5.0, 20.0);
}

// Задание №4
int convert_to_dec(const string& num, const int base)
{
    int accum = 0;
    for (int i = 0; i < num.length(); i++)
        accum = accum * base + (isdigit(num[i]) ? num[i] - '0' : toupper(num[i]) - 'A' + 10);
    return accum;
}

void task_4(char* file_name)
{
    ifstream in(file_name);
    ofstream out("C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\out4.txt");
    if (!(in.is_open() && out.is_open()))
        cout << "Ошибка открытия файла!";
    else
    {
        string input_str;
        int system_number;

        while (!in.eof())
        {
            in >> input_str;
            system_number = 0;
            for (auto symbol : input_str)
                if (!isalnum(symbol))
                {
                    system_number = -1;
                    break;
                }
                else if (system_number < (isdigit(symbol) ? symbol - '0' : toupper(symbol) - 'A' + 10))
                    system_number = (isdigit(symbol) ? symbol - '0' : toupper(symbol) - 'A' + 10);
            if (system_number == -1)
            {
                out << input_str << " - что-то непонятное\n";
                continue;
            }
            system_number++;
            out << input_str << "(" << system_number << ")" << " = " << convert_to_dec(input_str, system_number) << endl;
        }

        in.close();
        out.close();
        cout << "Запись в файл успешно завершена!";
    }
}

// Задание №5
string sum(int system_number, int count, ...)
{
    if (!count || (system_number <= 1) || (system_number > 36))
        return "Неправильные входные параметры!";
    va_list string_list;
    va_start(string_list, count);
    string str1 = va_arg(string_list, string), str2;

    if (count == 1)
    {
        va_end(string_list);
        return str1;
    }

    int product = 0, add = 0;

    for (int i = 1; i < count; i++)
    {
        str2 = va_arg(string_list, string);
        cout << str1 << " + " << str2;

        while (str1.length() < str2.length())
            str1.insert(str1.begin(), '0');
        while (str1.length() > str2.length())
            str2.insert(str2.begin(), '0');

        for (int i = str1.length() - 1; i >= 0; i--)
        {
            product = (isdigit(str1[i]) ? str1[i] - '0' : toupper(str1[i]) - 'A' + 10) + (isdigit(str2[i]) ? str2[i] - '0' : toupper(str2[i]) - 'A' + 10) + add;
            if (product >= system_number)
            {
                add = product / system_number;
                product %= system_number;
            }
            else
                add = 0;
            str1[i] = ((product < 10) ? product + '0' : product + 'A' - 10);
        }

        if (add)
            str1.insert(str1.begin(), ((add < 10) ? add + '0' : add + 'A' - 10));

        cout << " = " << str1 << endl;
    }

    int pos = 0;
    while ((str1[pos] == '0') && (pos < str1.length() - 1))
        pos++;
    if (pos)
        return str1.substr(pos, str1.length() - 1);
    va_end(string_list);
    return str1;
}

void task_5()
{
    cout << "\nИтог: " << sum(10, 2, (string)"10001", (string)"4340") << endl << endl;
    cout << "\nИтог: " << sum(2, 3, (string)"11", (string)"10001", (string)"101") << endl << endl;
    cout << "\nИтог: " << sum(2, 3, (string)"000000", (string)"10001", (string)"101") << endl << endl;
    cout << "\nИтог: " << sum(2, 2, (string)"000000", (string)"0");
}

// Задание №6
void input_operation(const char& aim, const char& cur_var_1, char& operation, const char& num)
{
    if ((aim != -1) && (cur_var_1 != -1))
    {
        if (!operation)
            operation = num;
        else
            cout << "\n'Повторный ввод операции!'\n";
    }
    else
        cout << "\n'Несоответствие инструкции!'\n";
}

void task_6(char *file_name)
{
    ifstream in(file_name);
    if (!in.is_open())
        cout << "Ошибка открытия файла!";
    else
    {
        char symbol, operation = 0, aim = -1, cur_var_1 = -1, cur_var_2 = -1;
        int com_count, variables[26];
        while ((symbol = in.get()) != EOF)
        {
            if (isspace(symbol))
                continue;
            if (isalpha(symbol))
            {
                if (cur_var_1 == -1)
                    cur_var_1 = toupper(symbol) - 'A';
                else if (cur_var_2 == -1)
                    cur_var_2 = toupper(symbol) - 'A';
                else
                    cout << "\n'В инструкции обнаружена лишняя переменная!'\n";
                continue;
            }
            switch (symbol)
            {
            case '%':
                while ((in.peek() != '\n') && (!in.eof()))
                    in.get();
                if (in.peek() == '\n')
                    in.get();
                break;
            case '{':
                com_count = 1;
                symbol = in.get();
                while (com_count && (!in.eof()))
                {
                    if (symbol == '{')
                        com_count++;
                    else if (symbol == '}')
                        com_count--;
                    symbol = in.get();
                }
                if (com_count != 0)
                    cout << "\n'Отсутствует символ завершения многострочного комментария!'\n";
                break;
            case '}':
                cout << "\n'Отсутствует символ начала многострочного комментария!'\n";
                break;
            case ';':
                if ((operation && (aim != -1) && (cur_var_1 != -1) && (cur_var_2 != -1)) || ((operation == 10) && (aim != -1) && (cur_var_1 != -1) && (cur_var_2 == -1)))
                {
/*
+ (дизъюнкция);                         1 !
& (конъюнкция);                         2 !
-> (импликация);                        3 !
<- (обратная импликация);               4 !
~ (эквиваленция);                       5 !
<> (сложение по модулю 2);              6 !
+> (коимпликация);                      7 !
? (штрих Шеффера);                      8 !
! (функция Вебба, стрелка Пирса);       9 !
\ (логическое отрицание);               10!
*/
                    switch (operation)
                    {
                    case 1:
                        variables[aim] = variables[cur_var_1] | variables[cur_var_2];
                        break;
                    case 2:
                        variables[aim] = variables[cur_var_1] & variables[cur_var_2];
                        break;
                    case 3:
                        variables[aim] = ~variables[cur_var_1] | variables[cur_var_2];
                        break;
                    case 4:
                        variables[aim] = variables[cur_var_1] | ~variables[cur_var_2];
                        break;
                    case 5:
                        variables[aim] = (~variables[cur_var_1] | variables[cur_var_2]) & (variables[cur_var_1] | ~variables[cur_var_2]);
                        break;
                    case 6:
                        variables[aim] = variables[cur_var_1] ^ variables[cur_var_2];
                        break;
                    case 7:
                        variables[aim] = variables[cur_var_1] & ~variables[cur_var_2];
                        break;
                    case 8:
                        variables[aim] = ~variables[cur_var_1] | ~variables[cur_var_2];
                        break;
                    case 9:
                        variables[aim] = ~variables[cur_var_1] & ~variables[cur_var_2];
                        break;
                    default:
                        variables[aim] = ~variables[cur_var_1]; //-(~variables[cur_var_1] + 1)
                        break;
                    }

                    cout << "[" << variables[aim] << "]";
                }
                else
                    cout << "\n'Введена несуществующая инструкция!'\n";
                operation = 0;
                aim = cur_var_1 = cur_var_2 = -1;
                break;
            case ':':
                if (in.peek() == '=')
                {
                    in.get();
                    if (aim != -1)
                        cout << "\n'Повторное присваивание в инструкции!'\n";
                    else
                    {
                        aim = cur_var_1;
                        cur_var_1 = -1;
                    }
                }
                break;
            case '+':
                if (in.peek() == '>')
                {
                    in.get();
                    input_operation(aim, cur_var_1, operation, 7);
                }
                else
                    input_operation(aim, cur_var_1, operation, 1);
                break;
            case '&':
                input_operation(aim, cur_var_1, operation, 2);
                break;
            case '-':
                if (in.peek() == '>')
                {
                    in.get();
                    input_operation(aim, cur_var_1, operation, 3);
                }
                else
                    cout << "\n'Введена несуществующая операция!'\n";
                break;
            case '<':
                if (in.peek() == '-')
                {
                    in.get();
                    input_operation(aim, cur_var_1, operation, 4);
                }
                else if (in.peek() == '>')
                {
                    in.get();
                    input_operation(aim, cur_var_1, operation, 6);
                }
                else
                    cout << "\n'Введена несуществующая операция!'\n";
                break;
            case '~':
                input_operation(aim, cur_var_1, operation, 5);
                break;
            case '?':
                input_operation(aim, cur_var_1, operation, 8);
                break;
            case '!':
                input_operation(aim, cur_var_1, operation, 9);
                break;
            case '\\':
                input_operation(aim, 0, operation, 10);
                break;
            default:
                cout << "\n'Введена несуществующая инструкция!'\n";
                break;
            }
        }

        cout << endl << endl;

        in.close();
        cout << "Работа интерпретатора завершена!";
    }
}

// Задание №7


// Задание №8