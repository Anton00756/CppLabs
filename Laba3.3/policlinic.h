#ifndef POLICLINIC
#define POLICLINIC
#include "logger.h"
#include <set>
#include <vector>

class HealthCenter
{
public:
	HealthCenter(int n, int m, int t)
	{
		N = n;
		for (int i = 0; i < m; i++)
			viewing.push_back(shared_ptr<Doctor>(new Doctor(i)));
		T = t;
		log = Logger::Builder().add_stream(Information, "policlinic_data.txt").build();
	}

	~HealthCenter()
	{
		free_docs.clear();
		viewing.clear();
		people_queue.clear();
		delete log;
	}

	void start();

private:
	struct Person
	{
		static int count;
		bool healthy;
		int ID, time_in, need_time;

		Person(int time, int need_t)
		{
			count++;
			ID = count;
			healthy = rand() % 2;
			time_in = time;
			need_time = need_t;
		}
	};

	struct Doctor
	{
		shared_ptr<Person> patient;
		shared_ptr<Doctor> helper;
		int work_time = 0, total_work_time = 0, ID;

		Doctor(int id)
		{
			ID = id;
		}
	};

	void spawn_person(int time);
	void desease(int time);
	void doc_work(int time);

	set<int> free_docs;
	vector<shared_ptr<Doctor>> viewing;
	vector<shared_ptr<Person>> viewing_people, people_queue;
	bool healty_patients;
	int N, T;
	Logger* log;
};

#endif