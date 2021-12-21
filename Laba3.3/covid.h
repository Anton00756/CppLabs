#ifndef COVID
#define COVID
#include "logger.h"

class Epidemic
{
public:
	class Population
	{
	public:
		Population(const string& file, const string& settings);
		string get_file() { return file_name; }

	private:
		struct Data
		{
			string id_list;
			unsigned long count = 1;

			Data(const string& str) { id_list = str; }
		};

		string file_name;
	};

	Epidemic(const string& file);
	void start();

private:
	struct Person
	{
		string name, surname;
		int disease_count = 0;
		bool sickness_state = false;
		vector<unsigned long> friends;
		
		Person(const string& n, const string& sn)
		{
			name = n;
			surname = sn;
		}
	};

	void disease(unsigned long ID);
	void recovery(shared_ptr<Person> man);

	bool repeated_incidence;
	int p_1, p_2, duration;
	vector<shared_ptr<Person>> people, patients;
};

#endif