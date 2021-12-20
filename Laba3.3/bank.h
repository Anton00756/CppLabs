#ifndef BANK
#define BANK
#include "logger.h"
#include "heaps.h"
#include <queue>

class Bank
{
private:
	struct Request
	{
		static int request_count;
		int urgency, time, to_office_id, difficulty, request_id;
		string information;

		Request()
		{
			request_count++;
			request_id = request_count;
		}
	};

	struct Employee
	{
		Request* job = nullptr;
		bool at_work = false, had_rest = false;
		int rest_time = 0, request_time = 0, arrival_time, home_time, ID;

		Employee(int id);
		bool go_rest(int cur_time);
	};

	struct Office
	{
		enum Logging { Start_Task, Finish_Task, Arriving, Leaving, Rest, Rest_End };
		Logger* logger;
		Heap<int, Request*>* wait_list;
		Comparer<int>* comp;
		vector<Employee*> staff;
		queue<Employee*> free_staff;
		static int office_id;
		int overload_time = 0, cur_staff = 0, staff_count = 0, wait_count = 0;

		Office(int k);
		~Office();
	};

	vector<Office*> offices;
	int t = 0, t_d = 0;
	double l = 0;
	int l_max = 0;

	void add_dep(int staff_count);
	void configure(int t, int t_d, int l_max, double l);
	void movement(int cur_time);
	void task_creator(int cur_time);
	void task_delivery(int cur_time);
	void working(int cur_time);
	Office* freest_office();

public:
	Bank()
	{
		int k_1 = 30, k_2 = 40, k_3 = 50;
		int t = 2, t_d = 1;
		double l = 0.15;
		int l_max = 2;

		add_dep(k_1);
		add_dep(k_2);
		add_dep(k_3);
		configure(t, t_d, l_max, l);
	}

	void opening();
	
	~Bank();
};

#endif