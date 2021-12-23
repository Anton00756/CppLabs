#include "policlinic.h"

int HealthCenter::Person::count = 0;

void HealthCenter::start()
{
	for (int i = 0; i < viewing.size(); i++)
		free_docs.insert(i);
	for (int time = 0; time < 1440; time++)
	{
		spawn_person(time);
		doc_work(time);
		desease(time);
	}

	int time = 1440;
	while (free_docs.size() != viewing.size())
	{
		doc_work(time);
		time++;
	}

	for (int i = 0; i < viewing.size(); i++)
		log->write(Information, "Doctor #" + to_string(i + 1) + " total work time: " + get_format_time(viewing[i]->total_work_time), time);
}

void HealthCenter::spawn_person(int time)
{
	if (!(rand() % 3))
	{
		people_queue.push_back(shared_ptr<Person>(new Person(time, rand() % T + 1)));
		log->write(Information, "New " + ((people_queue[people_queue.size() - 1]->healthy) ? string("H") : string("NH")) + " patient with ID [" + to_string(people_queue[people_queue.size() - 1]->ID) + "]", time);
	}
}

void HealthCenter::desease(int time)
{
	if (people_queue.size() > 1)
		if (!people_queue[0]->healthy)
			if (rand() % 2)
				for (auto& person : people_queue)
					if (person->healthy)
					{
						person->healthy = false;
						log->write(Information, "The person with ID [" + to_string(person->ID) + "] got infected in the queue!", time);
						break;
					}
}

void HealthCenter::doc_work(int time)
{
	for (int i = 0; (i < viewing_people.size()) && free_docs.size(); i++)
	{
		viewing[*free_docs.begin()]->patient = viewing_people[i];
		free_docs.erase(free_docs.begin());
		viewing_people.erase(viewing_people.begin() + i);
	}

	if (people_queue.size())
	{
		if ((free_docs.size() == viewing.size()) && (!viewing_people.size()))
			healty_patients = people_queue[0]->healthy;
		for (int i = 0; (i < people_queue.size()) && (free_docs.size() || (viewing_people.size() != N - viewing.size())); i++)
			if (people_queue[i]->healthy == healty_patients)
			{
				if (free_docs.size())
				{
					viewing[*free_docs.begin()]->patient = people_queue[i];
					free_docs.erase(free_docs.begin());
				}
				else
					viewing_people.push_back(people_queue[i]);
				people_queue.erase(people_queue.begin() + i);
			}
	}

	for (int i = 0; i < viewing.size(); i++)
		if (viewing[i]->patient)
		{
			viewing[i]->work_time++;
			viewing[i]->total_work_time++;
			if (viewing[i]->helper)
			{
				viewing[i]->helper->work_time++;
				viewing[i]->helper->total_work_time++;
			}
			else if (!(rand() % 5) && free_docs.size())
			{
				viewing[i]->helper = viewing[*free_docs.begin()];
				free_docs.erase(free_docs.begin());
				viewing[i]->helper->work_time++;
				viewing[i]->helper->total_work_time++;
			}
			if (viewing[i]->work_time == viewing[i]->patient->need_time)
			{
				log->write(Information, "Patient with ID [" + to_string(viewing[i]->patient->ID) + "] left. He was in clinic: " + to_string(time - viewing[i]->patient->time_in + 1) + " minutes", time);
				log->write(Information, "His medication time by Doctor #" + to_string(i + 1) + ": " + to_string(viewing[i]->work_time) + " minutes", time);
				viewing[i]->patient = nullptr;
				viewing[i]->work_time = 0;
				free_docs.insert(i);
				if (viewing[i]->helper)
				{
					log->write(Information, "Doctor #" + to_string(i + 1) + " was assisted by Doctor #" + to_string(viewing[i]->helper->ID + 1), time);
					viewing[i]->helper->work_time = 0;
					free_docs.insert(viewing[i]->helper->ID);
					viewing[i]->helper = nullptr;
				}
			}
		}
}