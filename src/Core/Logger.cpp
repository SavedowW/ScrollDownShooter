#include "Logger.h"

void Logger::print(const std::string &line)
{
#ifdef LOG_ENABLE
	std::cout << line;
#endif
}
