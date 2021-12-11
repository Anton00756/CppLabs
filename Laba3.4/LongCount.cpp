#include "LongCount.h"
#define MAX_VALUE 4294967296

void LongCount::increase_count(const int system, const int add_count)
{
    unsigned long long buf, add = add_count;
    for (int j = 0; j < value.size(); j++)
    {
        buf = unsigned long long(value[j]) * system + add;
        if (buf < MAX_VALUE)
        {
            value[j] = buf;
            add = 0;
        }
        else
        {
            add = buf / MAX_VALUE;
            value[j] = buf % MAX_VALUE;
            if (j + 1 == value.size())
            {
                value.push_back(add);
                break;
            }
        }
    }
}

LongCount::LongCount()
{
    value.push_back(0);
}

LongCount::LongCount(const string& count, const int system)
{
    if (count == "")
        throw std::exception("На вход передана пустая строка!");
    if ((system < 2) || (system > 36))
        throw std::exception("На вход передана неправильная СС!");
    sign = (count[0] == '-');
    value.push_back(0);
    for (int i = (sign) ? 1 : 0; i < count.length(); i++)
    {
        increase_count(system);
        increase_count(1, isdigit(count[i]) ? count[i] - '0' : toupper(count[i]) - 'A' + 10);
    }
}

LongCount::LongCount(const LongCount& object)
{
    sign = object.sign;
    value = object.value;
}

LongCount& LongCount::operator= (const LongCount& object)
{
    sign = object.sign;
    value.clear();
    value = object.value;
    return *this;
}

LongCount::~LongCount()
{
    value.clear();
}

ostream& operator<<(ostream& out, const LongCount& mycount)
{
    if (mycount.sign)
        out << "-";
    string in1 = to_string(mycount.value[mycount.value.size() - 1]), in2 = to_string(MAX_VALUE);
    for (int index = mycount.value.size() - 1; index > 0; index--)
        in1 = sum(production(in1, in2), to_string(mycount.value[index - 1]));
    out << in1;
    return out;
}

string sum(string in1, string in2)
{
    int product = 0, add = 0;

    while (in1.length() < in2.length())
        in1.insert(in1.begin(), '0');
    while (in1.length() > in2.length())
        in2.insert(in2.begin(), '0');

    for (int i = in1.length() - 1; i >= 0; i--)
    {
        product = in1[i] + in2[i] - 2 * '0' + add;
        if (product >= 10)
        {
            add = product / 10;
            product %= 10;
        }
        else
            add = 0;
        in1[i] = product + '0';
    }

    if (add)
    {
        in1.insert(in1.begin(), add + '0');
        add = 0;
    }
    return in1;
}

string production(string in1, string in2)
{
    string res = "", subres;
    int add = 0, product = 0, j = 0;
    for (int i = in2.length() - 1; i >= 0; i--)
    {
        subres = "";
        for (j = in1.length() - 1; j >= 0; j--)
        {
            product = (in1[j] - '0') * (in2[i] - '0') + add;
            if (product >= 10)
            {
                add = product / 10;
                product %= 10;
            }
            else
                add = 0;
            subres.insert(subres.begin(), product + '0');
        }
        if (add)
        {
            subres.insert(subres.begin(), add + '0');
            add = 0;
        }
        if (res == "")
            res = subres;
        else
        {
            for (j = 0; j < in2.length() - i - 1; j++)
                subres += '0';
            while (subres.length() != res.length())
                res.insert(res.begin(), '0');
            for (j = subres.length() - 1; j >= 0; j--)
            {
                product = subres[j] + res[j] - 2 * '0' + add;
                if (product >= 10)
                {
                    add = product / 10;
                    product %= 10;
                }
                else
                    add = 0;
                subres[j] = product + '0';
            }
            if (add)
            {
                subres.insert(subres.begin(), add + '0');
                add = 0;
            }
            res = subres;
        }
    }
    return res;
}

LongCount::LongCount(bool new_sign, const vector<unsigned long>& new_value)
{
    sign = new_sign;
    value = new_value;
}

LongCount LongCount::operator+ (const LongCount& b) const
{
    if (sign == b.sign)
    {
        unsigned long long buf = 0;
        int add = 0, new_length = max(value.size(), b.value.size());
        vector <unsigned long> answer = (value.size() >= b.value.size()) ? value : b.value;
        answer.resize(new_length, 0);

        for (int i = 0; i < min(value.size(), b.value.size()); i++)
        {
            buf = unsigned long long(value[i]) + unsigned long long(b.value[i]) + unsigned long long(add);
            if (buf < MAX_VALUE)
            {
                answer[i] = buf;
                add = 0;
            }
            else
            {
                answer[i] = buf % MAX_VALUE;
                add = buf / MAX_VALUE;
            }
        }

        if (add)
        {
            if (value.size() == b.value.size())
                answer.push_back(add);
            else
            {
                for (int i = min(value.size(), b.value.size()); i < new_length; i++)
                {
                    buf = unsigned long long(answer[i]) + unsigned long long(add);
                    if (buf < MAX_VALUE)
                    {
                        answer[i] = buf;
                        add = 0;
                        break;
                    }
                    else
                    {
                        answer[i] = buf % MAX_VALUE;
                        add = buf / MAX_VALUE;
                    }
                }

                if (add)
                    answer.push_back(add);
            }
        }

        return LongCount(sign, answer);
    }
    if (sign)
        return b - LongCount(false, value);
    return *this - LongCount(false, b.value);
}

LongCount LongCount::operator- (const LongCount& b) const
{
    if (sign != b.sign)
        return *this + LongCount(!b.sign, b.value);
    if ((sign == b.sign) && (value == b.value))
        return LongCount();

    unsigned long long buf = 0;
    int add = 0;
    vector <unsigned long> answer = (b < *this) ? value : b.value, *subtraction = const_cast<vector <unsigned long>*>((*this < b) ? &value : &b.value);

    for (int i = 0; i < min(value.size(), b.value.size()); i++)
    {
        if (answer[i] >= subtraction->at(i) + add)
        {
            answer[i] -= subtraction->at(i) + add;
            add = 0;
        }
        else
        {
            answer[i] = answer[i] + MAX_VALUE - subtraction->at(i) - add;
            add = 1;
        }
    }

    if (add)
    {
        for (int i = min(value.size(), b.value.size()); i < max(value.size(), b.value.size()); i++)
        {
            if (answer[i] >= add)
            {
                answer[i] -= add;
                add = 0;
                break;
            }
            else
            {
                answer[i] = answer[i] + MAX_VALUE - add;
                add = 1;
            }
        }
    }
    for (int i = answer.size() - 1; i >= 1; i--)
        if (!answer[i])
            answer.pop_back();
        else
            break;
    return LongCount(*this < b, answer);
}

LongCount LongCount::operator* (const LongCount& b) const
{
    vector <unsigned long> answer, *in1 = const_cast<vector <unsigned long>*>((value.size() >= b.value.size()) ? &value : &b.value), *in2 = const_cast<vector <unsigned long>*>((value.size() >= b.value.size()) ? &b.value : &value);
    int add = 0, length1 = max(value.size(), b.value.size()), length2 = min(value.size(), b.value.size());
    unsigned long long buf = 0;

    for (int i = 0; i < length2; i++)
    {
        for (int j = 0; j < length1; j++)
        {
            if (i + j >= answer.size())
                answer.push_back(0);
            buf = unsigned long long(in1->at(j)) * unsigned long long(in2->at(i)) + unsigned long long(add) + unsigned long long(answer[i + j]);
            if (buf >= MAX_VALUE)
            {
                add = buf / MAX_VALUE;
                answer[j + i] = buf % MAX_VALUE;
            }
            else
            {
                answer[j + i] = buf;
                add = 0;
            }
        }
    }
    if (add)
        answer.push_back(add);
    return LongCount(!(sign == b.sign), answer);
}

bool comparing(const vector <unsigned long>& a, const vector <unsigned long>& b)
{
    if (a.size() != b.size())
    {
        if (a.size() > b.size())
            return false;
    }
    else
    {
        for (int i = a.size() - 1; i >= 0; i--)
        {
            if (a[i] > b[i])
                return false;
            else if (a[i] == b[i])
            {
                if (!i)
                    return false;
            }
            else
                return true;
        }
    }
    return true;
}

bool LongCount::operator< (const LongCount& b) const
{
    if (sign != b.sign)
        if (sign)
            return true;
        else
            return false;
    else if (sign)
        return comparing(b.value, value);
    return comparing(value, b.value);
}

LongCount::LongCount(const LongCount::RandomCount& random)
{
    sign = random.sign;
    value.push_back(0);
    for (int i = (sign) ? 1 : 0; i < random.count.length(); i++)
    {
        increase_count(2);
        increase_count(1, isdigit(random.count[i]) ? random.count[i] - '0' : toupper(random.count[i]) - 'A' + 10);
    }
}

LongCount::RandomCount& LongCount::RandomCount::make(const int bits)
{
    sign = rand() % 2;
    count = "";
    for (int i = 0; i < bits; i++)
        count += (rand() % 2 + '0');
    return *this;
}


//LongCount::LongCount()
//{
//    sign = false;
//    length = 1;
//    value = new unsigned int[1]{ 0 };
//}
//
//LongCount::LongCount(bool signum, int new_length, unsigned int* arr)
//{
//    sign = signum;
//    length = new_length;
//    try
//    {
//        value = new unsigned int[length];
//    }
//    catch (...)
//    {
//        throw 777;
//    }
//    memcpy(value, arr, length * sizeof(unsigned int));
//}
//
//LongCount::LongCount(bool signum, string str)
//{
//    sign = signum;
//    length = 1;
//    value = new unsigned int[1]{ 0 };
//    unsigned long long input(0), buf(0);
//    int add(0);
//    bool expand(false);
//    for (int j = 0; j < str.length(); j++)
//    {
//        expand = false;
//        add = 0;
//        for (int i(0); i < length; i++)
//        {
//            buf = unsigned long long(value[i]) * 10 + unsigned long long(add);
//            if (buf < MAX_VALUE)
//            {
//                value[i] = value[i] * 10 + add;
//                add = 0;
//                expand = false;
//            }
//            else
//            {
//                add = buf / MAX_VALUE;
//                value[i] = buf % MAX_VALUE;
//                expand = true;
//            }
//        }
//
//        if (expand)
//        {
//            increase_arr(value, length);
//            value[length] = add;
//            length++;
//            expand = false;
//        }
//
//        add = str[j] - '0';
//        for (int i(0); i < length; i++)
//        {
//            buf = unsigned long long(value[i]) + unsigned long long(add);
//            if (buf < MAX_VALUE)
//            {
//                value[i] += add;
//                add = 0;
//                expand = false;
//                break;
//            }
//            else
//            {
//                add = buf / MAX_VALUE;
//                value[i] = buf % MAX_VALUE;
//                expand = true;
//            }
//        }
//
//        if (expand)
//        {
//            increase_arr(value, length);
//            value[length] = add;
//            length++;
//        }
//    }
//}
//
//LongCount LongCount::operator+ (const LongCount& b) const
//{
//    if (sign == b.sign)
//    {
//        unsigned int* res;
//        unsigned long long input(0);
//        int add = 0, new_length = max(length, b.length);
//        bool expand = false;
//        try
//        {
//            res = new unsigned int[new_length];
//        }
//        catch (...)
//        {
//            throw 777;
//        }
//        memcpy(res, (length >= b.length) ? value : b.value, new_length * sizeof(unsigned int));
//
//        for (int i = 0; i < min(length, b.length); i++)
//        {
//            input = unsigned long long(value[i]) + unsigned long long(b.value[i]) + unsigned long long(add);
//            if (input < MAX_VALUE)
//            {
//                res[i] = input;
//                add = 0;
//            }
//            else
//            {
//                res[i] = input % MAX_VALUE;
//                add = input / MAX_VALUE;
//            }
//        }
//
//        if (add)
//        {
//            if (length == b.length)
//            {
//                increase_arr(res, new_length);
//                res[new_length] = add;
//                new_length++;
//            }
//            else
//            {
//                for (int i = min(length, b.length); i < max(length, b.length); i++)
//                {
//                    input = unsigned long long(res[i]) + unsigned long long(add);
//                    if (input < MAX_VALUE)
//                    {
//                        res[i] = input;
//                        add = 0;
//                        break;
//                    }
//                    else
//                    {
//                        res[i] = input % MAX_VALUE;
//                        add = input / MAX_VALUE;
//                    }
//                }
//
//                if (add)
//                {
//                    increase_arr(res, new_length);
//                    res[new_length] = add;
//                    new_length++;
//                }
//            }
//        }
//
//        return LongCount(sign, new_length, res);
//    }
//    if (sign)
//        return b - LongCount(false, length, value);
//    return *this - LongCount(false, b.length, value);
//}
//
//LongCount LongCount::operator- (const LongCount& b) const
//{
//    if (sign != b.sign)
//        return *this + LongCount(!b.sign, b.length, b.value);
//    if (*this == b)
//        return LongCount();
//
//    unsigned int* res;
//    int add = 0, new_length = max(length, b.length);
//    try
//    {
//        res = new unsigned int[new_length];
//    }
//    catch (...)
//    {
//        throw 777;
//    }
//    memcpy(res, (*this > b) ? value : b.value, new_length * sizeof(unsigned int));
//    unsigned int* subtraction = (*this < b) ? value : b.value;
//
//    for (int i = 0; i < min(length, b.length); i++)
//    {
//        if (res[i] >= subtraction[i] + add)
//        {
//            res[i] -= subtraction[i] + add;
//            add = 0;
//        }
//        else
//        {
//            res[i] = res[i] + MAX_VALUE - subtraction[i] - add;
//            add = 1;
//        }
//    }
//
//    if (add)
//    {
//        for (int i = min(length, b.length); i < max(length, b.length); i++)
//        {
//            if (res[i] >= add)
//            {
//                res[i] -= add;
//                add = 0;
//                break;
//            }
//            else
//            {
//                res[i] = res[i] + MAX_VALUE - add;
//                add = 1;
//            }
//        }
//    }
//
//    return LongCount(*this < b, new_length, res);
//}
//
//LongCount LongCount::operator* (const LongCount& b) const
//{
//    return LongCount(!(sign == b.sign), production(convert_to_str(length, value), convert_to_str(b.length, b.value)));
//
//    unsigned int* res, * in1 = (length >= b.length) ? value : b.value, * in2 = (length >= b.length) ? b.value : value;
//    int add = 0, new_length = pow(min(length, b.length), 2) + 1, length1 = max(length, b.length), length2 = min(length, b.length);
//    unsigned long long product;
//    try
//    {
//        res = new unsigned int[new_length] {0};
//    }
//    catch (...)
//    {
//        throw 777;
//    }
//    for (int i = 0; i < length2; i++)
//    {
//        for (int j = 0; i < length1; i++)
//        {
//            product = unsigned long long(in1[j]) * unsigned long long(in2[i]) + unsigned long long(add) + unsigned long long(res[i + j]);
//            if (product >= MAX_VALUE)
//            {
//                add = product / MAX_VALUE;
//                res[j + i] = product % MAX_VALUE;
//            }
//            else
//            {
//                res[j + i] = product;
//                add = 0;
//            }
//        }
//    }
//    if (add)
//        res[new_length - 1] = add;
//    return LongCount(!(sign == b.sign), new_length, res);
//}
//
//void LongCount::operator= (const LongCount& b)
//{
//    sign = b.sign;
//    length = b.length;
//    delete[] value;
//    value = new unsigned int[length];
//    for (int i = 0; i < length; i++)
//        value[i] = b.value[i];
//}
//
//bool LongCount::operator< (const LongCount& b) const
//{
//    if (sign != b.sign)
//        if (sign)
//            return 1;
//        else
//            return 0;
//    else if (sign)
//    {
//        if (length != b.length)
//        {
//            if (length < b.length)
//                return 0;
//        }
//        else
//        {
//            for (int i = length - 1; i >= 0; i--)
//            {
//                if (value[i] > b.value[i])
//                    return 1;
//                else if (value[i] == b.value[i])
//                {
//                    if (!i)
//                        return 1;
//                }
//                else
//                    return 0;
//            }
//            return 0;
//        }
//        return 1;
//    }
//    else
//    {
//        if (length != b.length)
//        {
//            if (length < b.length)
//                return 1;
//        }
//        else
//        {
//            for (int i = length - 1; i >= 0; i--)
//            {
//                if (value[i] > b.value[i])
//                    return 0;
//                else if (value[i] == b.value[i])
//                {
//                    if (!i)
//                        return 0;
//                }
//                else
//                    return 1;
//            }
//            return 1;
//        }
//    }
//    return 0;
//}
//
//bool LongCount::operator> (const LongCount& b) const
//{
//    return !((*this < b) || (*this == b));
//}
//
//bool LongCount::operator<= (const LongCount& b) const
//{
//    return !(*this > b);
//}
//
//bool LongCount::operator>= (const LongCount& b) const
//{
//    return !(*this < b);
//}
//
//bool LongCount::operator== (const LongCount& b) const
//{
//    if ((sign == b.sign) && (length == b.length))
//    {
//        for (int i = 0; i < length; i++)
//            if (value[i] != b.value[i])
//                return 0;
//        return 1;
//    }
//    return 0;
//}
//
//bool LongCount::operator!= (const LongCount& b) const
//{
//    return !(*this == b);
//}
//
//LongCount::~LongCount()
//{
//    delete[] value;
//}
//
//ostream& operator<<(ostream& out, const LongCount& mycount)
//{
//    if (mycount.sign)
//        out << "-";
//    out << convert_to_str(mycount.length, mycount.value);
//    return out;
//}
//
//istream& operator>>(istream& in, LongCount& mycount)
//{
//    mycount.sign = false;
//    mycount.length = 1;
//    delete[] mycount.value;
//    mycount.value = new unsigned int[1]{ 0 };
//    unsigned long long input(0), buf(0);
//    int add(0);
//    bool expand(false);
//    if (in.peek() == '-')
//    {
//        in.get();
//        mycount.sign = true;
//    }
//    if (!((in.peek() >= '0') && (in.peek() <= '9')))
//        throw 1;
//    while ((in.peek() >= '0') && (in.peek() <= '9') && (!in.eof()))
//    {
//        expand = false;
//        add = 0;
//        for (int i(0); i < mycount.length; i++)
//        {
//            buf = unsigned long long(mycount.value[i]) * 10 + unsigned long long(add);
//            if (buf < MAX_VALUE)
//            {
//                mycount.value[i] = mycount.value[i] * 10 + add;
//                add = 0;
//                expand = false;
//            }
//            else
//            {
//                add = buf / MAX_VALUE;
//                mycount.value[i] = buf % MAX_VALUE;
//                expand = true;
//            }
//        }
//
//        if (expand)
//        {
//            increase_arr(mycount.value, mycount.length);
//            mycount.value[mycount.length] = add;
//            mycount.length++;
//            expand = false;
//        }
//
//        add = in.get() - '0';
//        for (int i(0); i < mycount.length; i++)
//        {
//            buf = unsigned long long(mycount.value[i]) + unsigned long long(add);
//            if (buf < MAX_VALUE)
//            {
//                mycount.value[i] += add;
//                add = 0;
//                expand = false;
//                break;
//            }
//            else
//            {
//                add = buf / MAX_VALUE;
//                mycount.value[i] = buf % MAX_VALUE;
//                expand = true;
//            }
//        }
//
//        if (expand)
//        {
//            increase_arr(mycount.value, mycount.length);
//            mycount.value[mycount.length] = add;
//            mycount.length++;
//        }
//    }
//
//    return in;
//}
//
//string convert_to_str(const int& length, const unsigned int* value)
//{
//    string in1 = to_string(value[length - 1]), in2 = to_string(MAX_VALUE);
//    for (int index = length - 1; index > 0; index--)
//        in1 = sum(production(in1, in2), to_string(value[index - 1]));
//    return in1;
//}
//
//string sum(string in1, string in2)
//{
//    int product = 0, add = 0;
//
//    while (in1.length() < in2.length())
//        in1.insert(in1.begin(), '0');
//    while (in1.length() > in2.length())
//        in2.insert(in2.begin(), '0');
//
//    for (int i = in1.length() - 1; i >= 0; i--)
//    {
//        product = in1[i] + in2[i] - 2 * '0' + add;
//        if (product >= 10)
//        {
//            add = product / 10;
//            product %= 10;
//        }
//        else
//            add = 0;
//        in1[i] = product + '0';
//    }
//    if (add)
//    {
//        in1.insert(in1.begin(), add + '0');
//        add = 0;
//    }
//    return in1;
//}
//
//string production(string in1, string in2)
//{
//    string res = "", subres;
//    int add = 0, product = 0, j = 0;
//    for (int i = in2.length() - 1; i >= 0; i--)
//    {
//        subres = "";
//        for (j = in1.length() - 1; j >= 0; j--)
//        {
//            product = (in1[j] - '0') * (in2[i] - '0') + add;
//            if (product >= 10)
//            {
//                add = product / 10;
//                product %= 10;
//            }
//            else
//                add = 0;
//            subres.insert(subres.begin(), product + '0');
//        }
//        if (add)
//        {
//            subres.insert(subres.begin(), add + '0');
//            add = 0;
//        }
//        if (res == "")
//            res = subres;
//        else
//        {
//            for (j = 0; j < in2.length() - i - 1; j++)
//                subres += '0';
//            while (subres.length() != res.length())
//                res.insert(res.begin(), '0');
//            for (j = subres.length() - 1; j >= 0; j--)
//            {
//                product = subres[j] + res[j] - 2 * '0' + add;
//                if (product >= 10)
//                {
//                    add = product / 10;
//                    product %= 10;
//                }
//                else
//                    add = 0;
//                subres[j] = product + '0';
//            }
//            if (add)
//            {
//                subres.insert(subres.begin(), add + '0');
//                add = 0;
//            }
//            res = subres;
//        }
//    }
//    return res;
//}
//
//void increase_arr(unsigned int*& arr, int length)
//{
//    unsigned int* new_arr;
//    try
//    {
//        new_arr = new unsigned int[length + 1];
//    }
//    catch (...)
//    {
//        throw 777;
//    }
//    memcpy(new_arr, arr, length * sizeof(unsigned int));
//    delete[] arr;
//    arr = new_arr;
//}