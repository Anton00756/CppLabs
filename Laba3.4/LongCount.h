#ifndef LongCountClass
#define LongCountClass
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stack>
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

private:
    bool sign = false;
    vector<unsigned long> value;

    void increase_count(const int system, const int add_count = 0);
    LongCount(bool new_sign, const vector<unsigned long>& new_value);
    bool operator< (const LongCount& b) const;
};

string sum(string in1, string in2);
string production(string in1, string in2);
#endif