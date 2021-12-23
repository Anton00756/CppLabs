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
	people_log = Logger::Builder().add_stream(Information, "lift_people_data.txt").build();
	for (int cur_time = 0; cur_time < 1440; cur_time++)
	{
		people_appearance(cur_time);
		people_distribution(cur_time);
		lift_movement(cur_time);
	}
	Logger* lifts_log = Logger::Builder().add_stream(Information, "lift_elevator_data.txt").build();
	for (int i = 0; i < lifts.size(); i++)
		lifts_log->write(Information, "Elevator #" + to_string(i + 1) + ": idle time = " + get_format_time(lifts[i]->idle_time) + ", work time = " + get_format_time(lifts[i]->travel_time) +
			".\nFloors count: " + to_string(lifts[i]->floors_count) + ". Sum weight: " + to_string(lifts[i]->sum_weight) + ". Max weight: " + to_string(lifts[i]->max_weight) +
			".\nOverload count: " + to_string(lifts[i]->overload_count) + ".");
}


void LiftSystem::people_appearance(int time)
{
	while (appearance.size() && (appearance[0]->time_of_appearance == time))
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
}

void LiftSystem::people_distribution(int time)
{
	shared_ptr<Lift> best_executer;
	int best_executer_floors;
	for (auto& task : waiting_floors)
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
	waiting_floors.clear();
}

void LiftSystem::lift_movement(int time)
{
	int second_runner;
	string person_friends;
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

		if (lift->state != Lift::Stay)
		{
			lift->travel_time++;
			second_runner = 0;
			while (second_runner < lift->floor_speed)
			{
				if (lift->state == Lift::GoUp)
				{
					if (!lift->pushed_buttons.size())
					{
						lift->state = Lift::Stay;
						break;
					}
					else if (*lift->pushed_buttons.rbegin() > lift->target_floor)
						lift->target_floor = *lift->pushed_buttons.rbegin();
					else if (*lift->pushed_buttons.rbegin() <= lift->cur_floor)
					{
						lift->state = Lift::GoDown;
						lift->target_floor = *lift->pushed_buttons.begin();
					}
				}
				else if (lift->state == Lift::GoDown)
				{
					if (!lift->pushed_buttons.size())
					{
						lift->state = Lift::Stay;
						break;
					}
					else if (*lift->pushed_buttons.begin() < lift->target_floor)
						lift->target_floor = *lift->pushed_buttons.begin();
					else if (*lift->pushed_buttons.rbegin() >= lift->cur_floor)
					{
						lift->state = Lift::GoUp;
						lift->target_floor = *lift->pushed_buttons.rbegin();
					}
				}
				else
					break;

				if (lift->pushed_buttons.count(lift->cur_floor))
				{
					for (int j = lift->passengers.size() - 1; j >= 0; j--)
						if (lift->passengers[j]->exit_floor == lift->cur_floor)
						{
							lift->cur_weight -= lift->passengers[j]->weight;
							lift->floor_speed = floor(60 / (3 + ceil(double(lift->cur_weight) / double(lift->max_mass))));
							person_friends = "";
							for (auto& friend_id : lift->passengers[j]->new_friends)
								person_friends += to_string(friend_id) + ((friend_id == *lift->passengers[j]->new_friends.rbegin()) ? "." : ", ");
							people_log->write(Information, "Person with ID [" + to_string(lift->passengers[j]->ID) + "] from " + to_string(lift->passengers[j]->spawn_floor) +
								" to " + to_string(lift->passengers[j]->exit_floor) + "\nEntering time: " + get_format_time(lift->passengers[j]->entering_time) +". Travel time: " +
								to_string(time - lift->passengers[j]->entering_time + 1) + " minute(-s). " + (lift->passengers[j]->overload_kick ? "Was overload kick" : "No overload kick") +
								(person_friends.size() ? ".\nNew friends IDs: " + person_friends : "."), time);
							lift->passengers.erase(lift->passengers.begin() + j);
						}

					if (floors[lift->cur_floor - 1]->wait_persons.size())
					{
						for (int j = 0; j < floors[lift->cur_floor - 1]->wait_persons.size(); j++)
							if (lift->cur_weight + floors[lift->cur_floor - 1]->wait_persons[j]->weight > lift->max_mass)
							{
								floors[lift->cur_floor - 1]->wait_persons[j]->overload_kick = true;
								lift->overload_count++;
								waiting_floors.insert(lift->cur_floor);
							}
							else
							{
								for (auto& person_in_lift : lift->passengers)
								{
									floors[lift->cur_floor - 1]->wait_persons[j]->new_friends.insert(person_in_lift->ID);
									person_in_lift->new_friends.insert(floors[lift->cur_floor - 1]->wait_persons[j]->ID);
								}

								lift->cur_weight += floors[lift->cur_floor - 1]->wait_persons[j]->weight;
								lift->sum_weight += floors[lift->cur_floor - 1]->wait_persons[j]->weight;
								if (lift->cur_weight > lift->max_weight)
									lift->max_weight = lift->cur_weight;
								lift->floor_speed = floor(60 / (3 + ceil(double(lift->cur_weight) / double(lift->max_mass))));

								floors[lift->cur_floor - 1]->wait_persons[j]->entering_time = time;
								lift->passengers.push_back(floors[lift->cur_floor - 1]->wait_persons[j]);
								lift->pushed_buttons.insert(floors[lift->cur_floor - 1]->wait_persons[j]->exit_floor);
								floors[lift->cur_floor - 1]->wait_persons.erase(floors[lift->cur_floor - 1]->wait_persons.begin() + j);
							}

						if (!floors[lift->cur_floor - 1]->wait_persons.size())
							lift->pushed_buttons.erase(lift->cur_floor);
					}

					lift->pushed_buttons.erase(lift->cur_floor);
				}

				if (lift->state == Lift::GoUp)
					lift->cur_floor++;
				else
					lift->cur_floor--;
				lift->floors_count++;
				second_runner++;
			}
		}
	}
}