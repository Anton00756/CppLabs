#ifndef Tasks
#define Tasks
#include <iostream>
#include <cstdarg>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

// Задание №1
int find_substr(const string& filename, const string& substr);
string searching(int (*fPtr)(const string&, const string&), const string& substr, int count, ...);
void task_1();
// Задание №2
bool camber_check(int count, ...);
void task_2();
// Задание №3
double decision(double x, int level, ...);
void task_3();
// Задание №4
int convert_to_dec(const string& num, const int base);
void task_4(char* file_name);
// Задание №5
string sum(int system_number, int count, ...);
void task_5();
// Задание №6 !!!
void input_operation(const char& aim, const char& cur_var_1, char& operation, const char& num);
void task_6(char* file_name);
// Задание №7 !!!
// Задание №8 !!!

#endif