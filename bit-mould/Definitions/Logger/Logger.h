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
	static std::ofstream m_log;
	static void Log(const char* info);
	static const char* GetTimestamp();
public:
	static void Log_(const char* info);
	static void EndLog();
	static void InitLogger(const char* filePath);
};