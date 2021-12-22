#include "elevator.h"

LiftSystem::LiftSystem(const string& properties, const string& people)
{
	ifstream input(properties);
	if (!input.is_open())
		throw std::exception("File opening error!");
	input >> n >> k;
	for (int i = 0; i < n; i++)
		floors.push_back(shared_ptr<Floor>(new Floor));
	int mass;
	for (int i = 0; i < k; i++)
	{
		input >> mass;
		lifts.push_back(shared_ptr<Lift>(new Lift(mass)));
	}
	input.close();

	input.open(people);
	if (!input.is_open())
		throw std::exception("File opening error!");
	string str;
	smatch m;
	while (getline(input, str))
	{
		regex_search(str, m, regex("^(\\d+) (\\d+) (\\d+):(\\d+) (\\d+) (\\d+)$"));
		appearance.push_back(shared_ptr<Person>(new Person(stoi(m[1]), stoi(m[3]) * 60 + stoi(m[4]), stoi(m[5]), stoi(m[6]), stoi(m[2]))));
	}
	sort(appearance.begin(), appearance.end(), [](shared_ptr<Person> a, shared_ptr<Person> b) {return a->time_of_appearance < b->time_of_appearance; });
	input.close();
}

void LiftSystem::start()
{
	shared_ptr<Lift> best_executer;
	int best_executer_floors;

	for (int cur_time = 0; cur_time < 1440; cur_time++)
	{
		while (appearance.size() && (appearance[0]->time_of_appearance == cur_time)) // появление
		{
			if (floors[appearance[0]->spawn_floor - 1]->wait_persons.size())
			{
				for (auto& meeting : floors[appearance[0]->spawn_floor - 1]->wait_persons)
				{
					appearance[0]->new_friends.insert(meeting->ID);
					meeting->new_friends.insert(appearance[0]->ID);
				}
			}
			floors[appearance[0]->spawn_floor - 1]->wait_persons.push_back(appearance[0]);
			waiting_floors.insert(appearance[0]->spawn_floor);
			appearance.erase(appearance.begin());
		}

		for (auto& task : waiting_floors) // распределение
		{
			best_executer = lifts[0];
			best_executer_floors = 10;
			for (auto& executer : lifts)
				if ((executer->state == Lift::Stay) || ((executer->state == Lift::GoUp) && (executer->cur_floor <= task)) || ((executer->state == Lift::GoDown) && (executer->cur_floor >= task)))
					if (abs(executer->cur_floor - task) < best_executer_floors)
					{
						best_executer_floors = abs(executer->cur_floor - task);
						best_executer = executer;
					}
			best_executer->pushed_buttons.insert(task);
		}

		for (auto& lift : lifts)
		{











			if (lift->state == Lift::Stay)
			{
				if (lift->pushed_buttons.size())
				{
					lift->target_floor = *lift->pushed_buttons.begin();
					if (lift->cur_floor > lift->target_floor)
						lift->state = Lift::GoDown;
					else
						lift->state = Lift::GoUp;
				}
				else
					lift->idle_time++;
			}
			else if (lift->state == Lift::GoUp)
			{
				if (lift->target_floor != *(lift->pushed_buttons.upper_bound(lift->target_floor)--))
					lift->target_floor = *(lift->pushed_buttons.upper_bound(lift->target_floor)--);
			}
			else
			{
				if (lift->target_floor != *lift->pushed_buttons.lower_bound(lift->target_floor))
					lift->target_floor = *lift->pushed_buttons.lower_bound(lift->target_floor);
			}

			if (lift->state != Lift::Stay)
			{
				for (vector<shared_ptr<Person>>::iterator waiter = floors[lift->cur_floor - 1]->wait_persons.begin(); waiter != floors[lift->cur_floor - 1]->wait_persons.end(); waiter++)
				{
					if (lift->cur_weight + (*waiter)->weight > lift->max_mass)
					{
						(*waiter)->overload_kick = true;
						lift->overload_count++;
						waiting_floors.insert(lift->cur_floor);
						continue;
					}

					for (auto& person_in_lift : lift->passengers)
					{
						(*waiter)->new_friends.insert(person_in_lift->ID);
						person_in_lift->new_friends.insert((*waiter)->ID);
					}

					lift->cur_weight += (*waiter)->weight;
					lift->sum_weight += (*waiter)->weight;
					if (lift->sum_weight > lift->max_weight)
						lift->max_weight = lift->sum_weight;
					lift->floor_speed = 3 + ceil(double(lift->cur_weight) / double(lift->max_mass));

					int cur_weight = 0; // текущий вес
					int floor_speed = 20; // проход одного пролёта в секундах
					int sum_weight = 0; // суммарная масса перевозок
					int max_weight = 0; // максимальный вес перевозки

					lift->passengers.push_back(*waiter);
					floors[lift->cur_floor - 1]->wait_persons.erase(waiter);
				}

				for (int way = 0; way < lift->floor_speed; way++)
				{
					


				}
			}
		}

		waiting_floors.clear();
			
	}
}