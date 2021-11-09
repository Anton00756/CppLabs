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
            out << input_str << "(" << system_number << ")" << " = " << convert_to_dec<int>(input_str, system_number) << endl;
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
        add = 0;
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
    cout << "\nИтог: " << sum(11, 2, (string)"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", (string)"1") << endl << endl;
    cout << "\nИтог: " << sum(36, 4, (string)"ZZZZZZZZZZ", (string)"1", (string)"0", (string)"1");
}

// Задание №6
template <typename T>
T convert_to_dec(const string& num, const int base)
{
    T accum = 0;
    for (int i = 0; i < num.length(); i++)
        accum = accum * base + (isdigit(num[i]) ? num[i] - '0' : toupper(num[i]) - 'A' + 10);
    return accum;
}

template <typename T>
string convert_from_dec(T num, const int base)
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
        char symbol, com_count;
        int instr_count = 1;
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
                            variables[toupper(m[2].str()[0]) - 'A'] = convert_to_dec<long long>(input, stoi(m[3].str()));
                        else
                            variables[toupper(m[2].str()[0]) - 'A'] = stoull(input);
                        if (detail)
                            to_out << instr_count << ") Ввод значения переменной '" << m[2] << "' в " << m[3] << "-й СС: " << input << endl;
                    }
                    else
                    {
                        cout << endl << "Значение '" << m[2] << "' в " << m[3] << "-й СС: ";
                        if (stoi(m[3].str()) != 10)
                            cout << convert_from_dec<long long>(variables[toupper(m[2].str()[0]) - 'A'], stoi(m[3].str())) << endl;
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
HashTable::HashTable()
{

}

HashTable::~HashTable()
{
    for (int i = 0; i < HASHSIZE; i++)
        if (table[i].next != nullptr)
        {
            HashNode* node = table[i].next, *old_node;
            while (node != nullptr)
            {
                old_node = node;
                node = node->next;
                delete old_node;
            }
        }
}

void HashTable::add(const string& key, const string& value)
{
    int hash_value = hash_func(key);
    if (table[hash_value].key == "")
    {
        table[hash_value].key = key;
        table[hash_value].value = value;
    }
    else
    {
        HashNode* node = table[hash_value].next;
        while (node->next != nullptr)
            node = node->next;
        node->next = new HashNode;
        node->next->key = key;
        node->next->value = value;
    }
}

string HashTable::find(const string& key)
{
    int hash_value = hash_func(key);
    if (table[hash_value].key != "")
    {
        HashNode* node = &table[hash_value];
        while ((node != nullptr) && (node->key != key))
            node = node->next;
        if (node == nullptr)
            return "";
        return node->value;
    }
    return "";
}

int HashTable::hash_func(const string& num)
{
    unsigned long long accum = 0;
    for (int i = 0; i < num.length(); i++)
        accum = accum * 62 + (isdigit(num[i]) ? num[i] - '0' : (isupper(num[i]) ? num[i] - 'A' + 10 : num[i] - 'a' + 36));
    return accum % HASHSIZE;
}

/*
#define hello no
#define me too
Bro , hello ! It was me !
Nice me ?
Hoho , hello
Yes !
*/

void task_7()
{
    string file_in = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in7.txt", file_buf = "C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\out7.txt";
    ifstream in(file_in);
    ofstream out(file_buf);
    if (!in.is_open() || !out.is_open())
        cout << "Ошибка открытия файла!\n";
    else
    {
        smatch m;
        string line;
        HashTable base;
        getline(in, line);
        while (regex_search(line, m, regex("^\\s*#define\\s*(\\w*)\\s*(\\S*)\\s*$")))
        {
            base.add(m[1].str(), m[2].str());
            getline(in, line);
        }
        while (!in.eof())
        {
            out << line << endl;
            getline(in, line);
        }
        out << line << endl;

        in.close();
        out.close();

        in.open(file_buf);
        out.open(file_in);
        if (!in.is_open() || !out.is_open())
            cout << "Ошибка открытия файла!\n";
        else
        {
            string result;
            char symbol;
            line = "";
            while ((symbol = in.get()) != EOF)
            {
                if (!isspace(symbol))
                    line += symbol;
                else
                {
                    if ((result = base.find(line)) == "")
                        out << line << symbol;
                    else
                        out << result << symbol;
                    line = "";
                }
            }

            in.close();
            out.close();

            cout << "Применение макрозамен завершено!";
        }
    }
}

// Задание №8
void add_op(int &aim, const int& arg1, const int& arg2)
{
    aim = arg1 + arg2;
}

void mult_op(int& aim, const int& arg1, const int& arg2)
{
    aim = arg1 * arg2;
}

void sub_op(int& aim, const int& arg1, const int& arg2)
{
    aim = arg1 - arg2;
}

void pow_op(int& aim, const int& arg1, const int& arg2)
{
    aim = pow(arg1, arg2);
}

void div_op(int& aim, const int& arg1, const int& arg2)
{
    if (!arg2)
        throw MyException("Деление на ноль запрещено!");
    aim = arg1 / arg2;
}

void rem_op(int& aim, const int& arg1, const int& arg2)
{
    if (arg2 <= 0)
        aim = arg1 % arg2;
    else
        cout << "Error: Для взятия по модулю введите положительное число!\n";
}

void xor_op(int& aim, const int& arg1, const int& arg2)
{
    aim = arg1 ^ arg2;
}

void bin_operation(map <string, int>& variables, map <string, void(*)(int& aim, const int& arg1, const int& arg2)>& func_base, const string& aim, const string& op, const string& ar1, const string& ar2)
{
    int arg1, arg2;
    if (regex_match(ar1, regex("^\\s*\\w+\\s*$")))
        arg1 = variables[ar1];
    else if (regex_match(ar1, regex("^\\s*-?\\d+\\s*$")))
        arg1 = stoi(ar1);
    else
        throw MyException("Ошибка ввода первого аргумента!");
    if (regex_match(ar2, regex("^\\s*\\w+\\s*$")))
        arg2 = variables[ar2];
    else if (regex_match(ar2, regex("^\\s*-?\\d+\\s*$")))
        arg2 = stoi(ar2);
    else
        throw MyException("Ошибка ввода первого аргумента!");
    if (func_base.find(op) == func_base.end())
        throw MyException("Команды '" + op + "' не существует!");
    func_base[op](variables[aim], arg1, arg2);
}

void task_8(const char* file_name)
{
    ifstream config_file(file_name);
    if (!config_file.is_open())
        cout << "Ошибка открытия config-файла!";
    else
    {
        char symbol, equal_pos = 'l', op_pos = 'l', in_out_pos = 'l'; // default: operation(ob_1, ob_2); left = ...
        string line = "", input_f_name = "input", output_f_name = "output", equal_sign = "=";
        smatch m;
        map <string, void(*)(int& aim, const int& arg1, const int& arg2)> func_base{ {"add", add_op}, {"mult", mult_op}, {"sub", sub_op}, {"pow", pow_op}, {"div", div_op}, {"rem", rem_op}, {"xor", xor_op} };

        while ((symbol = config_file.get()) != EOF)
        {
            switch (symbol)
            {
            case '#':
                while ((config_file.peek() != '\n') && (!config_file.eof()))
                    config_file.get();
                break;
            case '\n':
                if (line == "")
                    continue;
                if (regex_search(line, m, regex("^\\s*(left|right)\\s*=\\s*$")))
                    equal_pos = m[1].str()[0];
                else if (regex_match(line, regex("^(\\s|\\(|\\))*op(\\s|\\(|\\))*$")))
                {
                    if (regex_match(line, regex("^(\\s|\\(|\\))*\\(op\\)(\\s|\\(|\\))*$")))
                    {
                        op_pos = 'm';
                        if (regex_match(line, regex("^(\\s|\\()*\\(op\\)(\\s|\\(|\\))*$")))
                            in_out_pos = 'l';
                        else
                            in_out_pos = 'r';
                    }
                    else if (regex_match(line, regex("^(\\s|\\(|\\))*op\\s*$")))
                        op_pos = 'r';
                    else if (regex_match(line, regex("^\\s*op(\\s|\\(|\\))*$")))
                        op_pos = 'l';
                    else
                    {
                        cout << "Config Error: Настройка '" << line << "' не распознана!\n";
                        return;
                    }
                }
                else if (regex_search(line, m, regex("^\\s*(\\S+)\\s*(\\S+)\\s*$")))
                {
                    if (func_base.find(m[1].str()) != func_base.end())
                    {
                        func_base[m[2].str()] = func_base.at(m[1].str());
                        func_base.erase(m[1].str());
                    }
                    else if (m[1].str() == input_f_name)
                        input_f_name = m[2].str();
                    else if (m[1].str() == output_f_name)
                        output_f_name = m[2].str();
                    else if (m[1].str() == equal_sign)
                        equal_sign = m[2].str();
                    else
                    {
                        cout << "Config Error: Операции '" << m[1] << "' не существует!\n";
                        return;
                    }
                }
                else
                {
                    cout << "Config Error: Настройка '" << line << "' не распознана!\n";
                    return;
                }
                line = "";
                break;
            default:
                line += symbol;
                break;
            }
        }
        config_file.close();

        ifstream in("C:\\Users\\anton\\source\\repos\\Laba3.1\\Laba3.1\\in8.txt");
        if (!in.is_open())
            cout << "Ошибка открытия файла с инструкциями!";
        else
        {
            map <string, int> variables;
            string bin_regex_str, in_regex_str, out_regex_str;
            if (op_pos == 'l')
            {
                bin_regex_str = "\\s*(\\S+)\\s*\\(\\s*(\\S+)\\s*,\\s*(\\S+)\\s*\\)\\s*";
                in_regex_str = "\\s*" + input_f_name + "\\s*\\(\\s*(\\d*)\\s*\\)\\s*";
                out_regex_str = "^\\s*" + output_f_name + "\\s*\\(\\s*(\\w+)\\s*\\,?\\s*(\\d*)\\s*\\)\\s*$";
            }
            else if (op_pos == 'r')
            {
                bin_regex_str = "\\s*\\(\\s*(\\S+)\\s*,\\s*(\\S+)\\s*\\)\\s*(\\S+)\\s*";
                in_regex_str = "\\s*\\(\\s*(\\d*)\\s*\\)\\s*" + input_f_name + "\\s*";
                out_regex_str = "^\\s*\\(\\s*(\\w+)\\s*\\,?\\s*(\\d*)\\s*\\)\\s*" + output_f_name + "\\s*$";
            }
            else
            {
                bin_regex_str = "\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*";
                if (in_out_pos == 'l')
                    in_regex_str = "\\s*" + input_f_name + "\\s*(\\d*)\\s*";
                else
                    in_regex_str = "\\s*(\\d*)\\s*" + input_f_name + "\\s*";
                out_regex_str = "^\\s*\\s*(\\w*)\\s*" + output_f_name + "\\s*(\\w*)\\s*\\s*\\s*$";
            }
            if (equal_pos == 'l')
            {
                bin_regex_str = "^\\s*(\\w+)\\s*" + equal_sign + bin_regex_str + "$";
                in_regex_str = "^\\s*(\\w+)\\s*" + equal_sign + in_regex_str + "$";
            }
            else
            {
                bin_regex_str = "^" + bin_regex_str + equal_sign + "\\s*(\\w+)\\s*$";
                in_regex_str = "^" + in_regex_str + equal_sign + "\\s*(\\w+)\\s*$";
            }
            char com_count;
            line = "";

            int arg1, arg2, instr_count = 1;

            while ((symbol = in.get()) != EOF)
            {
                switch (symbol)
                {
                case '\n':
                    continue;
                    break;
                case '[':
                    com_count = 1;
                    symbol = in.get();
                    while (com_count && (!in.eof()))
                    {
                        if (symbol == '[')
                            com_count++;
                        else if (symbol == ']')
                            com_count--;
                        symbol = in.get();
                    }
                    if (com_count != 0)
                        cout << "Error: Отсутствует символ завершения многострочного комментария!\n";
                    break;
                case ']':
                    cout << "Error: Отсутствует символ начала многострочного комментария!\n";
                    break;
                case ';':
                    try
                    {
                        if (regex_search(line, m, regex(bin_regex_str))) // бинарные операции
                        {
                            if (equal_pos == 'l')
                            {
                                if (op_pos == 'l')
                                    bin_operation(variables, func_base, m[1].str(), m[2].str(), m[3].str(), m[4].str());
                                else if (op_pos == 'r')
                                    bin_operation(variables, func_base, m[1].str(), m[4].str(), m[2].str(), m[3].str());
                                else
                                    bin_operation(variables, func_base, m[1].str(), m[3].str(), m[2].str(), m[4].str());
                            }
                            else
                            {
                                if (op_pos == 'l')
                                    bin_operation(variables, func_base, m[4].str(), m[1].str(), m[2].str(), m[3].str());
                                else if (op_pos == 'r')
                                    bin_operation(variables, func_base, m[4].str(), m[3].str(), m[1].str(), m[2].str());
                                else
                                    bin_operation(variables, func_base, m[4].str(), m[2].str(), m[1].str(), m[3].str());
                            }
                        }
                        else if (regex_search(line, m, regex(in_regex_str))) // ввод
                        {
                            string aim = m[1].str(), sys = m[2].str();
                            if (equal_pos == 'r')
                                swap(sys, aim);
                            if ((sys == "") || (stoi(sys) == 10))
                            {
                                cout << instr_count << ") Введите значение '" << aim << "': ";
                                getline(cin, line);
                                if (!regex_match(line, regex("^\\s*-?\\d+\\s*$")))
                                    throw MyException("Ошибка ввода числа!");
                                variables[aim] = stoi(line);
                            }
                            else
                            {
                                if (!regex_match(sys, regex("^\\s*\\d+\\s*$")))
                                    throw MyException("Неверная система счисления!");
                                char system = stoi(sys);
                                if ((system < 2) || (system > 36))
                                    throw MyException("Неверная система счисления!");
                                cout << instr_count << ") Введите значение '" << aim << "' в " << (int)system << "-й СС: ";
                                getline(cin, line);
                                if (regex_search(line, m, regex("^\\s*(-?)(\\w+)\\s*$")))
                                {
                                    variables[aim] = convert_to_dec<int>(m[2].str(), system);
                                    if (m[1] == "-")
                                        variables[aim] *= -1;
                                }
                                else
                                    throw MyException("Некорректный ввод числа!");
                            }
                        }
                        else if (regex_search(line, m, regex(out_regex_str))) // вывод
                        {
                            string aim = m[1].str(), sys = m[2].str();
                            if ((op_pos == 'm') && (in_out_pos == 'l') && (m[1] == ""))
                                swap(aim, sys);
                            if ((sys == "") || (stoi(sys) == 10))
                                cout << instr_count << ") Значение '" << aim << "': " << variables[aim] << endl;
                            else
                            {
                                if (!regex_match(sys, regex("^\\s*\\d+\\s*$")))
                                    throw MyException("Неверная система счисления!");
                                char system = stoi(sys);
                                if ((system < 2) || (system > 36))
                                    throw MyException("Неверная система счисления!");
                                cout << instr_count << ") Значение '" << aim << "' в " << (int)system << "-й СС:" << ((variables[aim] > 0) ? " " : " -") << convert_from_dec<int>(abs(variables[aim]), system) << endl;
                            }
                        }
                        else
                            throw MyException("'" + line + "' - неизвестная инструкция!");
                    }
                    catch (MyException excep)
                    {
                        cout << instr_count << ") Error: " << excep.what() << endl;
                        return;
                    }
                    instr_count++;
                    line = "";
                    break;
                default:
                    line += symbol;
                    break;
                }
            }

            in.close();

            cout << "Работа интерпретатора завершена!";
        }
    }
}