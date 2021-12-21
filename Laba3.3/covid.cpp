#include "covid.h"

Epidemic::Population::Population(const string& file, const string& settings)
{
	ofstream out(file);
	if (!out.is_open())
		throw std::exception("File creating error!");
	file_name = file;
	unsigned long count = 100 + rand() % 9901, familiar_count;
	string name, surname, familiars;
	map <unsigned long, shared_ptr<Data>> base;
	shared_ptr<Data> cur_record;
	for (unsigned long i = 1; i < count; i++)
	{
		name = surname = familiars = "";
		familiar_count = 0;
		for (int j = 0; j < 5 + rand() % 3; j++)
			name += char('A' + rand() % 26);
		for (int j = 0; j < 7 + rand() % 3; j++)
			surname += char('A' + rand() % 26);
		if (base.find(i) != base.end())
		{
			cur_record = base.find(i)->second;
			base.erase(i);
			familiars = cur_record->id_list + ",";
		}
		if (i == count - 1)
			familiar_count = 1;
		else
			familiar_count = (rand() % (count - i - 1)) % 100 + 1;
		for (unsigned long j = 1; j <= familiar_count; j++)
		{
			familiars += to_string(i + j);
			if (j != familiar_count)
				familiars += ",";
			if (base.find(i + j) == base.end())
				base[i + j] = shared_ptr<Data>(new Data(to_string(i)));
			else
			{
				cur_record = base.find(i + j)->second;
				cur_record->count++;
				cur_record->id_list += "," + to_string(i);
			}
		}
		out << i << ',' << name << ',' << surname << ',' << familiars << endl;
	}
	name = surname = "";
	for (int j = 0; j < 5; j++)
		name += char('A' + rand() % 26);
	for (int j = 0; j < 7; j++)
		surname += char('A' + rand() % 26);
	cur_record = base.find(count)->second;
	out << count << ',' << name << ',' << surname << ',' << cur_record->id_list << endl;
	out << settings << endl;
	out.close();
}

Epidemic::Epidemic(const string& file)
{
	ifstream input(file);
	if (!input.is_open())
		throw std::exception("File creating error!");
	string str, str_count, count;
	smatch m;
	unsigned long i;
	while (getline(input, str))
	{
		if (regex_search(str, m, regex("^\\d+,([a-zA-Z]{5,7}),([a-zA-Z]{5,9}),([\\d+,]*)$")))
		{
			count = "";
			people.push_back(shared_ptr<Person>(new Person(m[1], m[2])));
			str_count = m[3];
			for (i = 0; i < str_count.length(); i++)
			{
				if (str_count[i] == ',')
				{
					people[people.size() - 1]->friends.push_back(stoul(count));
					count = "";
				}
				else
					count += str_count[i];
			}
			people[people.size() - 1]->friends.push_back(stoul(count));
		}
		else if(regex_search(str, m, regex("^([\\d|.]+) ([\\d|.]+) ([\\d]+) (\\w{4,5})$")))
		{
			p_1 = round(stod(m[1]) * 100);
			p_2 = round(stod(m[2]) * 100);
			duration = stoi(m[3]);
			if (m[4] == "true")
				repeated_incidence = true;
			else
				repeated_incidence = false;
		}
		else
			throw std::exception("Unknown data!");
	}
	input.close();
}

void Epidemic::start()
{
	unsigned long zero_patient = rand() % people.size(), patients_size;
	long j;
	disease(zero_patient);
	for (int i = 1; i < duration; i++)
	{
		patients_size = patients.size();
		for (j = 0; j < patients_size; j++) // болезнь
			for (auto& object : patients[j]->friends)
				if (!people[object]->sickness_state)
					if (repeated_incidence || (!repeated_incidence && !patients[object]->disease_count))
						if (rand() % 100 < p_1)
							disease(object);

		for (j = patients.size() - 1; j >= 0; j--) // выздоровление
			if (rand() % 100 < p_2)
			{
				recovery(patients[j]);
				patients.erase(patients.begin() + j);
			}

		if (!patients.size())
			break;
	}

	Logger* logger = Logger::Builder().add_stream(Information, "covid_out.txt").build();
	vector<shared_ptr<Person>> not_infected, recovered, friends_infected, all_friends_infected, several_sicknesses;
	bool break_flag;

	for (auto& object : people)
	{
		if (!object->sickness_state && !object->disease_count)
		{
			not_infected.push_back(object);
			break_flag = true;
			for (auto& friend_man : object->friends)
				if (!people[friend_man - 1]->sickness_state && !people[friend_man - 1]->disease_count)
				{
					break_flag = false;
					break;
				}
			if (break_flag)
				all_friends_infected.push_back(object);
		}
		if (object->disease_count)
		{
			recovered.push_back(object);
			for (auto& friend_man : object->friends)
				if (people[friend_man - 1]->sickness_state)
				{
					friends_infected.push_back(object);
					break;
				}
		}
		if (repeated_incidence)
			if (object->disease_count > 1)
				several_sicknesses.push_back(object);
	}

	logger->write(Information, "\t\t\t\tНе заразились:");
	for (auto& object : not_infected)
		logger->write(Information, object->name + " " + object->surname);
	logger->write(Information, "\t\t\t\tПереболели:");
	for (auto& object : recovered)
		logger->write(Information, object->name + " " + object->surname);
	logger->write(Information, "\t\t\t\tОкружение не исцелилось:");
	for (auto& object : friends_infected)
		logger->write(Information, object->name + " " + object->surname);
	logger->write(Information, "\t\t\t\tНе заразились, но всё окружение заражено:");
	for (auto& object : all_friends_infected)
		logger->write(Information, object->name + " " + object->surname);
	if (repeated_incidence)
	{
		logger->write(Information, "\t\t\t\tПереболели больше одного раза:");
		for (auto& object : several_sicknesses)
			logger->write(Information, object->name + " " + object->surname);
	}
}

void Epidemic::disease(unsigned long ID)
{
	people[ID]->sickness_state = true;
	patients.push_back(people[ID]);
}

void Epidemic::recovery(shared_ptr<Person> man)
{
	man->disease_count++;
	man->sickness_state = false;
}