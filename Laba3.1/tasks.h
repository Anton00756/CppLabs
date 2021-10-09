#ifndef Tasks
#define Tasks
#include <iostream>
#include <cstdarg>
#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
#include <regex>
using namespace std;

// Задание №1
int find_substr(const string& filename, const string& substr);
map <string, int> searching(int (*fPtr)(const string&, const string&), const string& substr, int count, ...);
void task_1();
// Задание №2
bool camber_check(int count, ...);
void task_2();
// Задание №3
double decision(double x, int level, ...);
void task_3();
// Задание №4
void task_4(char* file_name);
// Задание №5
string sum(int system_number, int count, ...);
void task_5();
// Задание №6
template <typename T>
T convert_to_dec(const string& num, const int base);
template <typename T>
string convert_from_dec(T num, const int base);
void task_6(char* file_name, ostream& to_out, bool detail = false);
// Задание №7
struct HashNode
{
	string key = "", value = "";
	HashNode* next = nullptr;
};

class HashTable
{
public:
	HashTable();
	~HashTable();
	void add(const string& key, const string& value);
	string find(const string& key);

private:
	static const int HASHSIZE = 128;
	HashNode table[HASHSIZE];

	int hash_func(const string& num);
};

void task_7();
// Задание №8
class MyException : public std::exception
{
private:
	std::string error_msg_str;

public:
	MyException(const std::string& err) : error_msg_str(err)
	{

	}
	const char* what() const noexcept { return error_msg_str.c_str(); }
};

void add_op(int& aim, const int& arg1, const int& arg2);
void mult_op(int& aim, const int& arg1, const int& arg2);
void sub_op(int& aim, const int& arg1, const int& arg2);
void pow_op(int& aim, const int& arg1, const int& arg2);
void div_op(int& aim, const int& arg1, const int& arg2);
void rem_op(int& aim, const int& arg1, const int& arg2);
void xor_op(int& aim, const int& arg1, const int& arg2);
void bin_operation(map <string, int>& variables, map <string, void(*)(int& aim, const int& arg1, const int& arg2)>& func_base, const string& aim, const string& op, const string& ar1, const string& ar2);
void task_8(const char* file_name);

#endif