#ifndef COMP
#define COMP
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Comparer
{
public:
	virtual ~Comparer() = default;
	virtual int compare(const T& left, const T& right) const = 0;
};

class IntComparer : public Comparer<int>
{
public:
	int compare(const int& left, const int& right) const override
	{
		if (left > right)
			return 1;
		else if (left < right)
			return -1;
		return 0;
	}
};

class StringComparer : public Comparer<string>
{
public:
	int compare(const string& left, const string& right) const override
	{
		for (int i = 0; i < max(left.size(), right.size()); i++)
		{
			if ((i == left.size()) || (left[i] < right[i]))
				return -1;
			if ((i == right.size()) || (left[i] > right[i]))
				return 1;
		}
		return 0;
	}
};

#endif