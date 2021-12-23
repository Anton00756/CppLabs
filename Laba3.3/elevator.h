#ifndef ELEVATOR
#define ELEVATOR
#include "logger.h"
#include <vector>
#include <set>

class LiftSystem
{
public:
	LiftSystem(const string& properties, const string& people);
	void start();

private:
	int n, k;

	struct Person
	{
		int ID, time_of_appearance, spawn_floor, exit_floor, weight, entering_time;
		set<int> new_friends;
		bool overload_kick = false;

		Person(int id, int time, int spawn, int exit, int mass)
		{
			ID = id;
			time_of_appearance = time;
			spawn_floor = spawn;
			exit_floor = exit;
			weight = mass;
		}
	};
	vector<shared_ptr<Person>> appearance;

	struct Floor
	{
		vector<shared_ptr<Person>> wait_persons;
	};
	vector<shared_ptr<Floor>> floors;

	struct Lift
	{
		enum lift_state {Stay, GoUp, GoDown};
		int state = Stay;
		set<int> pushed_buttons;
		vector<shared_ptr<Person>> passengers;
		int max_mass; // ������������ ����� �����
		int cur_weight = 0; // ������� ���
		int floor_speed = 20; // ������� �� ������
		int idle_time = 0; // ����� �������
		int travel_time = 0; // ����� � ����
		int floors_count = 0; // ������� ��������
		int sum_weight = 0; // ��������� ����� ���������
		int max_weight = 0; // ������������ ��� ���������
		int overload_count = 0; // ���������� ����������
		int cur_floor = 1; // ������� ����
		int target_floor = 1; // ������� ����

		Lift(int mass)
		{
			max_mass = mass;
		}
	};
	vector<shared_ptr<Lift>> lifts;
	set<int> waiting_floors;

	Logger* people_log;

	void people_appearance(int time);
	void people_distribution(int time);
	void lift_movement(int time);
};

#endif