#pragma once

#include <string>
#include <fstream>

#include "Util.h"

#define LOG_FILE_PATH "log.txt"

class Logger
{
private:
	Logger();
	~Logger();

	static Logger * logger;

	std::ofstream fileStream;

public:

	static Logger & getLogger();

	void log(const std::string & _a);

	template <typename T, typename... Args>
	void log(T _first, Args ... _args);
};

template<typename T, typename ...Args>
inline void Logger::log(T _first, Args ..._args)
{
	log(concatenate(_first, _args...));
}
