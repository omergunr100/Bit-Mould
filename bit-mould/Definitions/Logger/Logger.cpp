#include "Logger.h"

std::ofstream Logger::m_log;

void Logger::Log(const char * info)
{
	std::thread{ &Log_, std::ref(info) }.detach();
}

const char * Logger::GetTimestamp()
{
	std::time_t now = time(0);
	struct tm* localTime = localtime(&now);
	char buffer[32];
	strftime(buffer, sizeof buffer, " [%d-%m-%Y %H-%M-%S] ", localTime);
	return buffer;
}

void Logger::Log_(const char* info) {
	m_log << Logger::GetTimestamp() << info << std::endl;
}

void Logger::EndLog() {
	m_log.close();
}

void Logger::InitLogger(const char * filePath)
{
	std::time_t now = time(0);
	tm* localTime = localtime(&now);
	char buffer[32];
	std::stringstream fullPath;
	strftime(buffer, sizeof buffer, "[%d-%m-%Y %H-%M-%S] Log", localTime);
	fullPath << filePath << buffer << ".log";
	_mkdir(filePath);
	m_log.open(fullPath.str().c_str(), std::ios::out | std::ios::app);
	if (m_log.good()) {
		std::stringstream message;
		message << "Logger initialized at: " << fullPath.str().c_str();
		Log(message.str().c_str());
	}
	else {
		std::cerr << "Logger couldn't be initialized" << std::endl;
		exit(2);
	}
}