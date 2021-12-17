#include "LongCount.h"
#define MAX_VALUE 1000000
#define PI 3.14159265358979323846

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
    point = object.point;
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

string convert_to_str(const vector<unsigned long>& value)
{
    string in1 = to_string(value[value.size() - 1]), in2 = to_string(MAX_VALUE);
    for (int index = value.size() - 1; index > 0; index--)
        in1 = sum(production(in1, in2), to_string(value[index - 1]));
    return in1;
}

ostream& operator<<(ostream& out, const LongCount& mycount)
{
    string in = convert_to_str(mycount.value);
    if (mycount.sign)
        out << "-";
    if (mycount.point)
    {
        if (mycount.point * 6 <= in.length())
            in.insert(in.end() - mycount.point * 6, ',');
        else
        {
            in.insert(in.begin(), ',');
            in.insert(in.begin(), '0');
        }
    }
    out << in;
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
    vector <unsigned long> answer, * in1 = const_cast<vector <unsigned long>*>((value.size() >= b.value.size()) ? &value : &b.value), * in2 = const_cast<vector <unsigned long>*>((value.size() >= b.value.size()) ? &b.value : &value);
    int length1 = max(value.size(), b.value.size()), length2 = min(value.size(), b.value.size());
    unsigned long long buf = 0, add = 0;

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
        if (add)
        {
            answer.push_back(add);
            add = 0;
        }
    }
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

LongCount LongCount::operator% (const LongCount& b) const
{
    if (*this < b)
        return *this;
    if (*this == b)
        return LongCount();
    LongCount result;
    for (int i = value.size() - 1; i >= 0; i--)
    {
        result = result.shift(1);
        result.value[0] = value[i];
        while (!(result < b))
            result = result - b;
    }
    return result;
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
    count = "";
    for (int i = 0; i < bits; i++)
        count += (rand() % 2 + '0');
    return *this;
}

LongCount LongCount::shift(const int shift_length)
{
    if ((value.size() == 1) && !value[0])
        return LongCount();
    LongCount result(false, value);
    result.value.insert(result.value.begin(), shift_length, 0);
    return result;
}

LongCount LongCount::cut(int start, int stop) const
{
    if (value.size() <= start)
        return LongCount();
    else if (stop >= value.size())
        stop = value.size() - 1;
    return LongCount(false, vector <unsigned long>(value.begin() + start, value.begin() + stop + 1));
}

LongCount LongCount::karatsuba(const LongCount& b) const
{
    if ((value.size() == b.value.size()) && (value.size() == 1))
        return *this * b;
    int length = max(value.size(), b.value.size());
    if (length % 2)
        length++;
    LongCount a_part_1 = cut(length / 2, length - 1), a_part_2 = cut(0, length / 2 - 1), b_part_1 = b.cut(length / 2, length - 1), b_part_2 = b.cut(0, length / 2 - 1);
    LongCount result = a_part_1.karatsuba(b_part_1), last_prod = a_part_2.karatsuba(b_part_2);
    result = result.shift(length) + ((a_part_1 + a_part_2).karatsuba(b_part_1 + b_part_2) - result - last_prod).shift(length / 2) + last_prod;
    result.sign = !(sign == b.sign);
    return result;
}

void LongCount::Fourier_transform(vector<complex<double>>& a, bool invert) const
{
    int n = a.size();
    if (n == 1)
        return;

    vector<complex<double>> a0(n / 2), a1(n / 2);

    for (int i = 0, j = 0; i < n; i += 2, j++)
    {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    Fourier_transform(a0, invert);
    Fourier_transform(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    complex<double> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < n / 2; i++)
    {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];

        if (invert)
        {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }

        w *= wn;
    }
}

LongCount LongCount::Fourier_prod(const LongCount& b) const
{
    LongCount res;
    vector<complex<double>> fa(this->value.begin(), this->value.end()), fb(b.value.begin(), b.value.end());
    unsigned long n = pow(2, ceil(log2(max(value.size(), b.value.size()))) + 1);

    fa.resize(n);
    fb.resize(n);

    Fourier_transform(fa, false);
    Fourier_transform(fb, false);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    Fourier_transform(fa, true);
    unsigned long long buf;
    res.value.resize(n);
    for (int i = 0; i < n; i++)
    {
        buf = unsigned long long(abs(fa[i].real()) + 0.5) + res.value[i];
        for (int j = i; j < res.value.size(); j++)
        {
            if (buf < MAX_VALUE)
            {
                res.value[j] = buf;
                break;
            }
            else
            {
                res.value[j] = buf % MAX_VALUE;
                buf /= MAX_VALUE;
                if (j + 1 == res.value.size())
                {
                    res.value.push_back(buf);
                    break;
                }
            }
        }
    }
    for (int i = res.value.size() - 1; i >= 0; i--)
        if (!res.value[i])
            res.value.pop_back();
        else
            break;
    return res;
}

LongCount LongCount::degree(int grade) const
{
    if (grade < 0)
        throw std::exception("Степень должна быть не меньше нуля!");
    LongCount result("1"), buffer = *this;
    while (grade)
    {
        if (grade % 2)
            result = result.Fourier_prod(buffer);
        buffer = buffer.Fourier_prod(buffer);
        grade /= 2;
    }
    return result;
}

LongCount LongCount::mod_degree(int grade, const LongCount& mod_count) const
{
    if (sign || mod_count.sign)
        throw std::exception("Числа должны быть не меньше нуля!");
    else if (grade < 0)
        throw std::exception("Степень должна быть не меньше нуля!");
    LongCount result("1"), add = *this % mod_count;
    if ((add.value.size() == 1) && !add.value[0])
        add.value[0] = 1;
    for (int i = 0; i < grade; i++)
    {
        result = result * add;
        if (!(result < mod_count))
            result = result % mod_count;
        if ((result.value.size() == 1) && !result.value[0])
            result.value[0] = 1;
    }
    return result;
}

bool LongCount::operator== (const LongCount& b) const
{
    if ((sign == b.sign) && (value.size() == b.value.size()))
    {
        for (int i = 0; i < value.size(); i++)
            if (value[i] != b.value[i])
                return false;
        return true;
    }
    return false;
}

LongCount LongCount::operator/ (const LongCount& b) const
{
    if ((b.value.size() == 1) && !b.value[0])
        throw std::exception("Деление на ноль запрещено!");
    if (value.size() < b.value.size())
        return LongCount();

    int iter = 0, n = b.value.size() + 1;;
    if (value.size() - b.value.size() > 1)
        iter = ceil(log2(value.size() - b.value.size()));
    iter += 1;
    unsigned long long tmp = pow(MAX_VALUE, 3), tmp2 = MAX_VALUE * unsigned long long(*(b.value.end() - 1));
    if (b.value.size() > 1)
        tmp2 += *(b.value.end() - 2);
    tmp /= tmp2;
    LongCount answer(to_string(tmp));

    for (int i = 0; i != iter; i++)
    {
        answer = ((LongCount("2").shift(n)) - (b * answer)) * answer;
        n *= 2;
        unsigned int best_n = std::pow(2, i) + b.value.size() + 1;
        answer.point += n - best_n;
        n = best_n;
    }

    answer = answer * *this;
    answer.point += n;

    if (((answer + LongCount("1")) * b < *this) || ((answer + LongCount("1")) * b == *this))
        answer = answer + LongCount("1");
    return answer;
}

LongCount gcd(const LongCount& a, const LongCount& b)
{
    LongCount first = a, second = b;
    first.sign = second.sign = false;
    while (!(first == second))
    {
        if (first < second)
            second = second - first;
        else
            first = first - second;
    }
    return first;
}

LongCount LongCount::int_part() const
{
    if ((value.size() == 1) && !value[0])
        return LongCount();
    LongCount result = *this;
    result = result + LongCount("1").shift(point - 1);
    result.value = vector <unsigned long>(result.value.begin() + point, result.value.end());
    if (!result.value.size())
        return LongCount();
    result.point = 0;
    return result;
}

LC_struct ext_gcd(LongCount a, LongCount b)
{
    LC_struct answer;
    LongCount q, aa, bb("1"), buffer;
    answer.first_coef = LongCount("1");
    answer.second_coef = LongCount();
    while ((b.value.size() != 1) || b.value[0])
    {
        LongCount mod_res = a % b;
        if ((mod_res == LongCount()) && (a == b))
            break;
        q = (a / b).int_part();
        buffer = a;
        a = b;
        b = buffer - b * q;
        buffer = answer.first_coef;
        answer.first_coef = aa;
        aa = buffer - aa * q;
        buffer = answer.second_coef;
        answer.second_coef = bb;
        bb = buffer - bb * q;
    }
    answer.gcd = a;
    return answer;
}

LongCount bin_gcd(const LongCount& a, const LongCount& b)
{
    LongCount first = a, second = b, coef("1");
    while (!((first == LongCount()) || (second == LongCount())))
    {
        while ((first % LongCount("2") == LongCount()) && (second % LongCount("2") == LongCount()))
        {
            first = (first / LongCount("2")).int_part();
            second = (second / LongCount("2")).int_part();
            coef = coef * LongCount("2");
        }
        while (first % LongCount("2") == LongCount())
            first = (first / LongCount("2")).int_part();
        while (second % LongCount("2") == LongCount())
            second = (second / LongCount("2")).int_part();
        if (!(first < second))
            first = first - second;
        else 
            second = second - first;
    }
    return second * coef;
}

bool LongCount::simple() const
{
    LongCount check("3");
    while (!(*this < check.Fourier_prod(check)))
        if (*this % check == LongCount())
            return false;
        else
            check = check + LongCount("1");
    return true;
}

int Legendre_symbol(const LongCount& a, const LongCount& b)
{
    if (a.point || a.sign || (a == LongCount()))
        throw std::exception("Числитель не является целым числом!");
    if (b.point || !(LongCount("2") < b) || b.sign)
        throw std::exception("Знаменатель не является простым числом!");
    if (b % LongCount("2") == LongCount())
        throw std::exception("Знаменатель является чётным числом!");
    if (!b.simple())
        throw std::exception("Знаменатель не является простым числом!");
    LongCount comp = a % b;
    if (comp == LongCount())
        return 0;
    LongCount check = LongCount("1");
    while (check < b)
        if (check.Fourier_prod(check) % b == comp)
            return 1;
        else
            check = check + LongCount("1");
    return -1;
}

int Jacobi_symbol(const LongCount& a, const LongCount& b)
{
    if (a.point || a.sign || (a == LongCount()))
        throw std::exception("Числитель не является целым числом!");
    if (b.point || !(LongCount("1") < b))
        throw std::exception("Знаменатель не больше единицы!");
    if (b % LongCount("2") == LongCount())
        throw std::exception("Знаменатель является чётным числом!");
    if (b.simple())
        return Legendre_symbol(a, b);
    int result = 1;
    LongCount multiplier = LongCount("3"), count = b;
    while (!(multiplier == count))
    {
        if (multiplier.simple())
            while (count % multiplier == LongCount())
            {
                result *= Legendre_symbol(a, multiplier);
                if (!result)
                    return 0;
                count = (count / multiplier).int_part();
            }
        multiplier = multiplier + LongCount("2");
    }
    return result;
}

bool Fermat_test(const LongCount& count, int accuracy)
{
    if (count % LongCount("2") == LongCount())
        return true;
    if ((count.value.size() == 1) && (count.value[0] < accuracy + 2))
        accuracy = count.value[0] - 2;
    LongCount comparing_value("2");
    int degree = stoi(convert_to_str(count.value)) - 1;
    for (int i = 0; i < accuracy; i++)
    {
        if (!(gcd(comparing_value, count) == LongCount("1")))
            return false;
        if (!(comparing_value.mod_degree(degree, count) == LongCount("1")))
            return false;
        comparing_value = comparing_value + LongCount("1");
    }
    return true;
}

bool SoloveyStrassen_test(const LongCount& count, int accuracy)
{
    if (count < LongCount("3"))
        throw std::exception("Число должно быть больше 2!");
    if (count % LongCount("2") == LongCount())
        return true;
    if ((count.value.size() == 1) && (count.value[0] < accuracy + 2))
        accuracy = count.value[0] - 2;
    LongCount comparing_value("2");
    int degree = stoi(convert_to_str(count.value)) - 1;
    for (int i = 0; i < accuracy; i++)
    {
        if (LongCount("1") < gcd(comparing_value, count))
            return false;
        if (!(comparing_value.mod_degree(degree, count) == LongCount("1")))
            return false;
        comparing_value = comparing_value + LongCount("1");
    }
    return true;
}

double SoloveyStrassen_probability(int k)
{
    return (1 - pow(2, -k)) * 100;
}

bool MillerRabin_test(const LongCount& count, int accuracy)
{
    if (count < LongCount("4"))
        throw std::exception("Число должно быть больше 3!");
    if (count % LongCount("2") == LongCount())
        return true;
    LongCount t = count - LongCount("1");
    int s = 0;
    while (t % LongCount("2") == LongCount())
    {
        t = (t / LongCount("2")).int_part();
        s += 1;
    }
    if ((count.value.size() == 1) && (count.value[0] < accuracy + 3))
        accuracy = count.value[0] - 3;
    LongCount comparing_value("2"), x;
    bool out = false;
    int degree = stoi(convert_to_str(count.value)) - 1;
    for (int i = 0; i < accuracy; i++)
    {
        x = comparing_value.mod_degree(stoi(convert_to_str(t.value)), count);
        if ((x == LongCount("1")) || (x == count - LongCount("1")))
        {
            comparing_value = comparing_value + LongCount("1");
            continue;
        }
        for (int j = 0; j < s - 1; j++)
        {
            x = x.Fourier_prod(x) % count;
            if (x == LongCount("1"))
                return false;
            if (x == count - LongCount("1"))
            {
                out = true;
                break;
            }
        }
        if (out)
        {
            comparing_value = comparing_value + LongCount("1");
            out = false;
            continue;
        }
        return false;
    }
    return true;
}

double MillerRabin_probability(int k)
{
    return (1 - pow(4, -k)) * 100;
}