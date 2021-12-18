#ifndef LOGGER
#define LOGGER
#include "json.hpp"
#include <fstream>
#include <regex>
#include "comp.h"

enum access { Trace, Debug, Information, Warning, Error, Fatal };

class Stream
{
public:
	friend class Logger;
	Stream(access access_mode, const string& file_name)
	{
		this->access_mode = access_mode;
		if (file_name == "stdout")
			return;
		out_stream.open(file_name, ofstream::app);
		if (!out_stream.is_open())
			throw std::exception("Невозможно открыть файл!");
	}

	~Stream()
	{
		out_stream.close();
	}

private:
	ofstream out_stream;
	access access_mode;
	Stream* next = nullptr;;
};

class Logger
{
public:
	class Builder
	{
	public:
		Builder() {}
		Builder& add_stream(access access_mode, const string& conf_file);
		Stream* build();
		Stream* build(const string& config_file);

	private:
		bool check_config(const nlohmann::json& file);
		vector <Stream*> streams;
	};

	void write(access access_mode, const string& text, const string& time);
	~Logger();
	Logger(Stream* first_stream) { streams = first_stream; }

private:
	Stream* streams = nullptr;
};
#endif