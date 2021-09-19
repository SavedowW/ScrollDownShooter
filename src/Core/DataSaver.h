#ifndef DATA_SAVER_H_
#define DATA_SAVER_H_
#include <fstream>
#include "Logger.h"
#include <string>
#include <unordered_map>
#include <string_view>

class DataSaver
{
public:
	DataSaver(const std::string &fileName_);
	void save();
	bool load();

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_variables;

private:
	std::string m_fileName;
};

#endif