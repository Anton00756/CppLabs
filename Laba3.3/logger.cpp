#include "logger.h"
#include <set>

Logger::Builder& Logger::Builder::add_stream(access access_mode, const string& conf_file)
{
    shared_ptr<Stream> new_stream(new Stream(access_mode, conf_file));
    if (streams.size() != 0)
        streams[streams.size() - 1]->next = new_stream;
    streams.push_back(new_stream);
    return *this;
}

Logger* Logger::Builder::build()
{
    if (!streams.size())
        return new Logger(nullptr);
    return new Logger(streams[0]);
}

Logger* Logger::Builder::build(const string& config_file)
{
    ifstream f(config_file, ifstream::binary);
    if (!f.is_open())
        throw std::exception("���������� ������� config-file!");

    nlohmann::json js, js_streams;
    f >> js;

    if (!check_config(js))
        throw std::exception("������������ ������������ ����� json!");
    js_streams = js.at("streams");

    string file_name = js_streams[0].at("filename").get<string>();
    access access_mode = js.at("access_value").at(js_streams[0].at("access").get<string>());

    shared_ptr<Stream> new_stream(new Stream(access_mode, file_name));
    if (streams.size() != 0)
        streams[streams.size() - 1]->next = new_stream;
    streams.push_back(new_stream);

    for (int i = 1; i < js_streams.size(); i++)
    {
        file_name = js_streams[i].at("filename").get<string>();
        access_mode = js.at("access_value").at(js_streams[i].at("access").get<string>());
        new_stream = shared_ptr<Stream>(new Stream(access_mode, file_name));
        streams[streams.size() - 1]->next = new_stream;
        streams.push_back(new_stream);
    }
    return new Logger(streams[0]);
}

bool Logger::Builder::check_config(const nlohmann::json& j)
{
    set <string> check_list;
    string file_name;
    try
    {
        j.at("streams");
        j.at("access_value");
        nlohmann::json j2 = j.at("streams");
        if (j2.empty())
            throw std::exception("json ����!");
        for (int i = 0; i < j2.size(); i++)
        {
            if (j2[i].size() != 2)
                throw std::exception("������������ ������������!");
            file_name = j2[i].at("filename").get<string>();
            if (check_list.count(file_name))
                throw std::exception("������ ����� ��� ����������!");
            check_list.insert(file_name);
            if (regex_match(j2[i].at("access").get<string>(), regex("debug|fatal|error|information|warning|trace")) == false)
                throw std::exception("�������������� ������� �����������!");
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

void Logger::write(access access_mode, const string& text)
{
    shared_ptr<Stream> tmp = streams;
	string access;
	switch (access_mode)
	{
	case Trace:
        access = "Trace";
		break;
	case Debug:
        access = "Debug";
		break;
	case Information:
        access = "Information";
		break;
	case Warning:
        access = "Warning";
		break;
	case Error:
        access = "Error";
		break;
	case Fatal:
        access = "Fatal";
		break;
	default:
		throw std::exception("������ �������!");
	}

    char buffer[80];
    time_t seconds;
    tm timeinfo;
    seconds = time(NULL);
    localtime_s(&timeinfo, &seconds);
    strftime(buffer, 80, "%A, %d %B %Y, %H:%M:%S", &timeinfo);
    string time(buffer);

	while (tmp != nullptr)
	{
        if (access_mode >= tmp->access_mode)
        {
            if (tmp->out_stream.is_open())
                tmp->out_stream << "[" << time << "][" << access << "]: " << text << endl;
            else
                cout << "[" << time << "][" << access << "]: " << text << endl;
        }
		tmp = tmp->next;
	}
}

void Logger::write(access access_mode, const string& text, int time_value)
{
    shared_ptr<Stream> tmp = streams;
    string access;
    switch (access_mode)
    {
    case Trace:
        access = "Trace";
        break;
    case Debug:
        access = "Debug";
        break;
    case Information:
        access = "Information";
        break;
    case Warning:
        access = "Warning";
        break;
    case Error:
        access = "Error";
        break;
    case Fatal:
        access = "Fatal";
        break;
    default:
        throw std::exception("������ �������!");
    }

    string time = ((time_value / 60 < 10) ? ("0" + to_string(time_value / 60)) : to_string(time_value / 60)) + ":" + ((time_value % 60 < 10) ? ("0" + to_string(time_value % 60)) : to_string(time_value % 60));

    while (tmp != nullptr)
    {
        if (access_mode >= tmp->access_mode)
        {
            if (tmp->out_stream.is_open())
                tmp->out_stream << "[" << time << "][" << access << "]: " << text << endl;
            else
                cout << "[" << time << "][" << access << "]: " << text << endl;
        }
        tmp = tmp->next;
    }
}