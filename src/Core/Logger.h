#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <iostream>
#define LOG_ENABLE

class Logger
{
public:
	static void print(const std::string &line);

};

#endif