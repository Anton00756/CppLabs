#ifndef LongCountClass
#define LongCountClass
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stack>
#include <complex>
using namespace std;

class LongCount
{
public:
    class RandomCount
    {
        friend class LongCount;
    public:
        RandomCount() {}
        ~RandomCount() {}

        RandomCount& make(const int bits);

    private:
        bool sign = false;
        string count;
    };

    LongCount();
    LongCount(const string& count, const int system = 10);
    LongCount(const LongCount& object);
    LongCount(const RandomCount& random);
    LongCount& operator= (const LongCount& object);
    ~LongCount();

    friend ostream& operator<<(ostream& out, const LongCount& mycount);

    LongCount operator+ (const LongCount& b) const;
    LongCount operator- (const LongCount& b) const;
    LongCount operator* (const LongCount& b) const;
    LongCount operator/ (const LongCount& b) const;
    LongCount int_part() const;
    bool operator< (const LongCount& b) const;

    LongCount karatsuba(const LongCount& b) const;
    LongCount Fourier_prod(const LongCount& b) const;
    LongCount degree(int grade) const;
    LongCount mod_degree(int grade, const LongCount& mod_count) const;
    
    friend LongCount gcd(const LongCount& a, const LongCount& b);
    typedef struct gcd_str LC_struct;
    friend LC_struct ext_gcd(const LongCount& a, const LongCount& b);
    friend LongCount bin_gcd(const LongCount& a, const LongCount& b);

    friend int Legendre_symbol(const LongCount& a, const LongCount& b);
    friend int Jacobi_symbol(const LongCount& a, const LongCount& b);
    friend bool Fermat_test(const LongCount& count, int accuracy);
    friend bool SoloveyStrassen_test(const LongCount& count, int accuracy);
    friend bool MillerRabin_test(const LongCount& count, int accuracy);

private:
    bool sign = false;
    vector<unsigned long> value;
    int point = 0;

    void increase_count(const int system, const int add_count = 0);
    LongCount(bool new_sign, const vector<unsigned long>& new_value);
    bool operator== (const LongCount& b) const;
    LongCount operator% (const LongCount& b) const;

    void Fourier_transform(vector<complex<double>>& a, bool invert) const;
    LongCount shift(const int shift_length);
    LongCount cut(int start, int stop) const;
    bool simple() const;
};

typedef struct gcd_str
{
    LongCount gcd, first_coef, second_coef;
} LC_struct;

string sum(string in1, string in2);
string production(string in1, string in2);

LongCount gcd(const LongCount& a, const LongCount& b);
LC_struct ext_gcd(const LongCount& a, const LongCount& b);
LongCount bin_gcd(const LongCount& a, const LongCount& b);

int Legendre_symbol(const LongCount& a, const LongCount& b);
int Jacobi_symbol(const LongCount& a, const LongCount& b);
bool Fermat_test(const LongCount& count, int accuracy = 100);
bool SoloveyStrassen_test(const LongCount& count, int accuracy = 100);
double SoloveyStrassen_probability(int k = 100);
bool MillerRabin_test(const LongCount& count, int accuracy = 100);
double MillerRabin_probability(int k = 100);
#endif