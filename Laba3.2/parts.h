#ifndef PARTS
#define PARTS
#include <iostream>
#include<vector>
#include<string>
#include<stack>
using namespace std;

template<typename T> class Comparer
{
public:
	virtual int ñompare(const T& left, const T& right) = 0;
	virtual ~Comparer() = default;
};

class IntComparer : public Comparer<int>
{
public:
	~IntComparer() override;
	int ñompare(const int& left, const int& right) override;
};

template<typename TKey, typename TValue> class Ass_Cont
{
public:
	virtual void insert(const TKey& key, const TValue& value) = 0;
	virtual TValue remove(const TKey& key) = 0;
	virtual TValue& find(const TKey& key) = 0;
	virtual ~Ass_Cont() = default;
};

#endif