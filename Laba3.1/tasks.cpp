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

map <string, int> searching(int (*fPtr)(const string&, const string&), const string& substr, int count, ...)
{
    va_list string_list;
    va_start(string_list, count);
    string filename;
    int number;
    map <string, int> result;
    for (int i = 1; i <= count; i++)
    {
        filename = va_arg(string_list, string);
        number = fPtr(filename, substr);
        if (number != -1)
            result[filename] = number;
    }
    va_end(string_list);
    return result;
}

void task_1()
{
    string name1 = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in1_1.txt";
    string name2 = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in1_2.txt";
    string name3 = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in1_3.txt";
    map <string, int> result = searching(find_substr, "hel lo", 3, name1, name2, name3);
    map <string, int> ::iterator it = result.begin();
    while (it != result.end())
    {
        cout << "В файле '" << it->first << "' найдена искомая строка. Позиция: " << it->second << ".\n";
        it++;
    }
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
    cout << "\nИтог: " << sum(11, 2, (string)"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", (string)"1");
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

long long conv_to_dec(const string& num, const int base)
{
    long long accum = 0;
    for (int i = 0; i < num.length(); i++)
        accum = accum * base + (isdigit(num[i]) ? num[i] - '0' : toupper(num[i]) - 'A' + 10);
    return accum;
}

string conv_from_dec(long long num, const int base)
{
    int r;
    string output;
    while (num)
    {
        r = num % base;
        output.insert(output.begin(), (r > 9) ? (r - 10 + 'A') : (r + '0'));
        num /= base;
    }
    return output;
}

void task_6(char *file_name, ostream& to_out, bool detail)
{
    ifstream in(file_name);
    if (!in.is_open())
        cout << "Ошибка открытия входного файла!\n";
    else
    {
        bool error;
        char symbol;
        int com_count, instr_count = 1;
        long long variables[26] = { 0 };
        string instruction = "", input;
        smatch m;
        while ((symbol = in.get()) != EOF)
        {
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
                {
                    if (detail)
                        to_out << instr_count << ") ";
                    to_out << "Error: Отсутствует символ завершения многострочного комментария!\n";
                }
                break;
            case '}':
                if (detail)
                    to_out << instr_count << ") ";
                to_out << "Error: Отсутствует символ начала многострочного комментария!\n";
                break;
            case ';':
                if (regex_search(instruction, m, regex("^\\s{0,}(read|write)\\s{0,}\\(\\s{0,}(\\w{1})\\s{0,}\\,\\s{0,}(\\d{1,2})\\s{0,}\\)\\s{0,}$"))) // read / write
                {
                    if (m[1] == "read")
                    {
                        cout << endl << "Введите значение '" << m[2] << "' в " << m[3] << "-й СС: ";
                        getline(cin, input);
                        if (stoi(m[3].str()) != 10)
                            variables[toupper(m[2].str()[0]) - 'A'] = conv_to_dec(input, stoi(m[3].str()));
                        else
                            variables[toupper(m[2].str()[0]) - 'A'] = stoull(input);
                        if (detail)
                            to_out << instr_count << ") Ввод значения переменной '" << m[2] << "' в " << m[3] << "-й СС: " << input << endl;
                    }
                    else
                    {
                        cout << endl << "Значение '" << m[2] << "' в " << m[3] << "-й СС: ";
                        if (stoi(m[3].str()) != 10)
                            cout << conv_from_dec(variables[toupper(m[2].str()[0]) - 'A'], stoi(m[3].str())) << endl;
                        else
                            cout << variables[toupper(m[2].str()[0]) - 'A'] << endl;
                        if (detail)
                            to_out << instr_count << ") Вывод значения переменной '" << m[2] << "' в " << m[3] << "-й СС: " << variables[toupper(m[2].str()[0]) - 'A'] << endl;
                    }
                }
                else if (regex_search(instruction, m, regex("^\\s{0,}(\\w{1})\\s{0,}:=\\s{0,}(\\w{1})\\s{0,}([^\\w\\s]{1,2})\\s{0,}(\\w{1})\\s{0,}$"))) // бинарная операция
                {
                    error = false;
                    if (m[3].str() == "+")
                        variables[toupper(m[1].str()[0]) - 'A'] = variables[toupper(m[2].str()[0]) - 'A'] | variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "&")
                        variables[toupper(m[1].str()[0]) - 'A'] = variables[toupper(m[2].str()[0]) - 'A'] & variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "->")
                        variables[toupper(m[1].str()[0]) - 'A'] = ~variables[toupper(m[2].str()[0]) - 'A'] | variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "<-")
                        variables[toupper(m[1].str()[0]) - 'A'] = variables[toupper(m[2].str()[0]) - 'A'] | ~variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "~")
                        variables[toupper(m[1].str()[0]) - 'A'] = (~variables[toupper(m[2].str()[0]) - 'A'] | variables[toupper(m[4].str()[0]) - 'A']) & (variables[toupper(m[2].str()[0]) - 'A'] | ~variables[toupper(m[4].str()[0]) - 'A']);
                    else if (m[3].str() == "<>")
                        variables[toupper(m[1].str()[0]) - 'A'] = variables[toupper(m[2].str()[0]) - 'A'] ^ variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "+>")
                        variables[toupper(m[1].str()[0]) - 'A'] = variables[toupper(m[2].str()[0]) - 'A'] & ~variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "?")
                        variables[toupper(m[1].str()[0]) - 'A'] = ~variables[toupper(m[2].str()[0]) - 'A'] | ~variables[toupper(m[4].str()[0]) - 'A'];
                    else if (m[3].str() == "!")
                        variables[toupper(m[1].str()[0]) - 'A'] = ~variables[toupper(m[2].str()[0]) - 'A'] & ~variables[toupper(m[4].str()[0]) - 'A'];
                    else
                    {
                        to_out << "Error: Инструкция не распознана!\n";
                        error = true;
                    }
                    if (!error && detail)
                        to_out << instr_count << ") " << m[1] << " := " << m[2] << " " << m[3] << " " << m[4] << " = " << variables[toupper(m[1].str()[0]) - 'A'] << endl;
                }
                else if (regex_search(instruction, m, regex("^\\s{0,}(\\w{1})\\s{0,}:=\\s{0,}\\\\\\s{0,}(\\w{1})\\s{0,}$"))) // унарная операция
                {
                    variables[toupper(m[1].str()[0]) - 'A'] = ~variables[toupper(m[2].str()[0]) - 'A']; //-(~variables[cur_var_1] + 1)
                    if (detail)
                        to_out << instr_count << ") " << m[1] << " := \\" << m[2] << " = " << variables[toupper(m[1].str()[0]) - 'A'] << endl;
                }
                else
                    to_out << "Error: Инструкция не распознана!\n";
                instr_count++;
                instruction = "";
                break;
            default:
                instruction += symbol;
                break;
            }
        }

        in.close();
        cout << "Работа интерпретатора завершена!";
    }
}

// Задание №7


// Задание №8