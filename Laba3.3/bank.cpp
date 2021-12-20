#include "bank.h"

int Bank::Request::request_count = 0, Bank::Office::office_id = 0;

Bank::Employee::Employee(int id)
{
	ID = id;
	arrival_time = rand() % 240;
	home_time = 540 + (rand() % 240);
}

bool Bank::Employee::go_rest(int cur_time)
{
	if (had_rest)
		return false;
	int rest_probability = rand() % 100;
	if ((rest_probability < 15) || ((cur_time >= 240 + arrival_time) && (rest_probability < 75)))
	{
		rest_time = 60;
		had_rest = true;
		return true;
	}
	return false;
}

Bank::Office::Office(int k)
{
	office_id++;
	comp = new IntComparer;
	logger = Logger::Builder().add_stream(Information, "log_of_office¹" + to_string(office_id) + ".txt").build();
	wait_list = new BinomialHeap<int, Request*>(comp);
	staff_count = k;
	for (int i = 0; i < k; i++)
		staff.push_back(new Employee(i));
}

Bank::Office::~Office()
{
	delete logger;
	while (true)
	{
		try
		{
			delete wait_list->del_min();
		}
		catch (...)
		{
			break;
		}
	}
	delete wait_list;
	delete comp;
	for (auto item : staff)
		delete item;
}

void Bank::add_dep(int staff_count)
{
	offices.push_back(new Office(staff_count));
}

void Bank::configure(int t, int t_d, int l_max, double l)
{
	this->t = t;
	this->t_d = t_d;
	this->l_max = l_max;
	this->l = l;
}

void Bank::opening()
{
	int cur_time = 0;

	for (int i = 0; i < offices.size(); i++)
		offices[i]->logger->write(Information, "opening", cur_time);
	while (cur_time != 780)
	{
		movement(cur_time);
		task_creator(cur_time);
		task_delivery(cur_time);
		working(cur_time);
		cur_time++;
	}

	for (auto office : offices)
	{
		while (true)
		{
			try
			{
				Request* task = office->wait_list->del_min();
				office->logger->write(Information, "request " + to_string(task->request_id) + " wasn't done", 11 * 60);
				delete task;
			}
			catch (...)
			{
				break;
			}
		}
		office->logger->write(Information, "closing", cur_time);
	}
}

void Bank::movement(int cur_time)
{
	if (cur_time < 240)
	{
		for (int i = 0; i < offices.size(); i++)
		{
			for (int j = 0; j < offices[i]->staff.size(); j++)
			{
				if (offices[i]->staff[j]->arrival_time == cur_time)
				{
					Employee* worker = offices[i]->staff[j];
					worker->at_work = true;
					offices[i]->free_staff.push(worker);
					offices[i]->cur_staff++;
					offices[i]->logger->write(Information, "Worker " + to_string(worker->ID) + " arrived", cur_time);
				}
			}
		}
	}
	else if (cur_time >= 540)
	{
		for (int i = 0; i < offices.size(); i++)
		{
			for (int j = 0; j < offices[i]->staff.size(); j++)
			{
				if (offices[i]->staff[j]->home_time == cur_time)
				{
					Employee* worker = offices[i]->staff[j];
					if (worker->request_time)
					{
						offices[i]->wait_list->insert(worker->job->time, worker->job);
						worker->job = nullptr;
						worker->request_time = 0;
					}
					worker->at_work = false;
					offices[i]->cur_staff--;
					offices[i]->logger->write(Information, "Worker " + std::to_string(worker->ID) + " left", cur_time);
				}
			}
		}
	}
}

void Bank::task_creator(int cur_time)
{
	bool create_task;
	int overload_probability = rand() % 100;
	Office* free_office;
	while (create_task = rand() % 2)
	{
		Request* task = new Request;
		task->urgency = rand() % 10;
		task->time = cur_time;
		task->to_office_id = rand() % offices.size();
		task->difficulty = rand() % 6;
		task->information = "!!! EXPRESS # " + to_string(task->request_id) + " !!!";

		if (overload_probability < l * 100)
		{
			free_office = freest_office();
			free_office->wait_list->merge(offices[task->to_office_id]->wait_list);
			offices[task->to_office_id]->overload_time = 60 + (rand() % 121);
		}

		if (offices[task->to_office_id]->overload_time)
		{
			free_office = freest_office();
			free_office->wait_list->insert(task->urgency, task);
			free_office->wait_count++;
			free_office->logger->write(Information, "request received", cur_time);
		}
		else
		{
			offices[task->to_office_id]->wait_list->insert(task->urgency, task);
			offices[task->to_office_id]->wait_count++;
			offices[task->to_office_id]->logger->write(Information, "request " + to_string(task->request_id) + " received", cur_time);
			cout << "request " + to_string(task->request_id) + " received " << task->to_office_id << endl;
		}
	}
}

void Bank::task_delivery(int cur_time)
{
	Employee* worker;
	bool sign;
	for (int i = 0; i < offices.size(); i++)
	{
		if (offices[i]->overload_time)
			continue;
		while (!offices[i]->free_staff.empty() && offices[i]->wait_count)
		{
			worker = offices[i]->free_staff.front();
			while (!worker->at_work)
			{
				offices[i]->free_staff.pop();
				if (!offices[i]->free_staff.empty())
					worker = offices[i]->free_staff.front();
				else
				{
					worker = nullptr;
					break;
				}
			}
			if (worker == nullptr)
				break;
			worker->job = offices[i]->wait_list->del_min();
			offices[i]->free_staff.pop();
			offices[i]->wait_count--;
			sign = rand() % 2;
			worker->request_time = t + (sign ? t_d : -t_d) + worker->job->difficulty * 2;
			offices[i]->logger->write(Information, "Worker " + to_string(worker->ID) + " started task ¹" + to_string(worker->job->request_id), cur_time);
		}
	}
}

void Bank::working(int cur_time)
{
	Employee* worker;
	for (int i = 0; i < offices.size(); i++)
	{
		if (offices[i]->overload_time)
		{
			offices[i]->overload_time--;
			if (!offices[i]->overload_time)
				offices[i]->logger->write(Information, "office continued working", cur_time);
			else
				continue;
		}
		for (int j = 0; j < offices[i]->staff.size(); j++)
		{
			worker = offices[i]->staff[j];
			if (worker->at_work)
			{
				if (!worker->rest_time)
				{
					if (worker->request_time)
					{
						worker->request_time--;
						if (!worker->request_time)
						{
							offices[i]->logger->write(Information, "Worker " + to_string(worker->ID) + " finished request " + to_string(worker->job->request_id), cur_time);
							delete worker->job;
							worker->job = nullptr;
							if (worker->go_rest(cur_time))
								offices[i]->logger->write(Information, "Worker [" + to_string(worker->ID) + "] went to lunch", cur_time);
							else
								offices[i]->free_staff.push(worker);
						}
					}
				}
				else
				{
					worker->rest_time--;
					if (!worker->rest_time)
					{
						offices[i]->free_staff.push(worker);
						offices[i]->logger->write(Information, "Worker " + to_string(worker->ID) + " stopped resting", cur_time);
					}
				}
			}
		}
	}
}

Bank::Office* Bank::freest_office()
{
	Office* free_office = offices[0];
	for (int i = 1; i < offices.size(); i++)
	{
		if ((offices[i]->wait_count / (offices[i]->cur_staff + 1)) < (free_office->wait_count / (free_office->cur_staff + 1)))
			free_office = offices[i];
	}
	return free_office;
}

Bank::~Bank()
{
	for (auto i : offices)
		delete i;
}