//https://github.com/Anton00756/CppLabs.git
#include <iostream>
//#include <cstdarg>
//#include <stdio.h>
//#include <fstream>
#include <string>
//#include <map>
//#include <regex>
using namespace std;

#include <iostream>
#include <stack>

template<typename tkey> class comparer
{
public:
	virtual int compare(const tkey& left, const tkey& right) = 0;
	virtual ~comparer() = default;
};

template<typename tkey, typename tvalue> class assoc_container
{
public:
	virtual void insert(const tkey& key, const tvalue& value) = 0;
	virtual tvalue remove(const tkey& key) = 0;
	virtual tvalue& find(const tkey& key) = 0;
	virtual ~assoc_container() = default;
};

template<typename tkey, typename tvalue>
class binary_search_tree : public assoc_container<tkey, tvalue>
{
protected:
	struct node
	{
		tkey key;
		tvalue value;
		node* left;
		node* right;
	};
private:
	node* _root;
protected:
	class insertion_template
	{
	public:
		enum status
		{
			ok,
			duplicate_key
		};
	public:
		status invoke(node*& root, const tkey& key, const tvalue& value)
		{
			std::pair<std::stack<node*>, bool> way = find_way(key);
			if (way.second)
			{
				// TODO: stopped here
				way.first.top().right = new node(){ key = key, value = value, left = nullptr, right = nullptr };
			}
		}
	private:
		std::pair<std::stack<node*>, bool> find_way(const tkey& key)
		{

		}
	protected:

	};

	class removing_template
	{

	};

	class finding_template
	{

	};
private:
	insertion_template _inserter;
	removing_template _remover;
	finding_template _finder;
protected:
	binary_search_tree(
		insertion_template inserter,
		removing_template remover,
		finding_template finder,
		comparer<tkey>* comparer)
		: _inserter(inserter), _remover(remover), _finder(finder), _comparer(comparer)
	{

	}
public:
	binary_search_tree(comparer<tkey>* comparer)
		: binary_search_tree(insertion_template(), removing_template(), finding_template(), comparer)
	{

	}
public:
	void insert(const tkey& key, const tvalue& value) override
	{
		_inserter(_root, key, value);
	}

	tvalue remove(const tkey& key) override
	{
		throw std::exception();
	}

	tvalue& find(const tkey& key) override
	{
		throw std::exception();
	}
};

int main(int argc, char* argv[])
{
    system("chcp 1251>nul");

    cout << "\tЛабораторная работа №3.1\n\tМ8О-211Б-20: Комиссаров Антон\n\n";

    

    system("pause>nul");
}