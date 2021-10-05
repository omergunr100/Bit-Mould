#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <map>
#include <direct.h>
#include <thread>
#include "../Config/Config.h"



class Logger {
	Logger() = delete;
	Logger(const Logger& other) = delete;
	~Logger() = delete;
private:
	static const std::map<int, std::string> priority;
public:
	const enum PRIORITY { None = 0, Low = 1, Normal = 2, High = 3, Critical = 4 };
private:
	static std::ofstream m_log;
	static void Log(PRIORITY pr, const char* info);

public:
	static void Log_(PRIORITY pr, const char* info);
	static std::string GetPriority(PRIORITY pr);
	static void EndLog();
	static int InitLogger(const char* filePath);
};