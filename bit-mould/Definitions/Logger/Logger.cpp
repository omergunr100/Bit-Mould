#include "Logger.h"

const std::map<int, std::string> Logger::priority{ {0, "[None] "}, {1, "[Low] "}, {2, "[Normal] "}, {3, "[High] "}, {4, "[Critical] "} };
std::ofstream Logger::m_log;

std::string Logger::GetPriority(PRIORITY pr) {
	return priority.at(pr);
}

void Logger::Log(PRIORITY pr, const char* info) {
	std::time_t now = time(0);
	tm* localTime = localtime(&now);
	char buffer[32];
	strftime(buffer, sizeof buffer, " [%d-%m-%Y %H-%M-%S %Z] ", localTime);
	std::cout << GetPriority(pr) << buffer << info << std::endl;
	m_log << GetPriority(pr) << buffer << info << std::endl;
}

void Logger::EndLog() {
	m_log.close();
}

int Logger::InitLogger(const char * filePath)
{
	std::time_t now = time(0);
	tm* localTime = localtime(&now);
	char buffer[32];
	std::stringstream fullPath;
	strftime(buffer, sizeof buffer, "[%d-%m-%Y %H-%M-%S] Log", localTime);
	fullPath << filePath << buffer << ".log";
	m_log.open(fullPath.str().c_str());
	if (m_log.good()) {
		std::stringstream message;
		message << "Logger initialized at: " << fullPath.str().c_str();
		Log(PRIORITY::Normal, message.str().c_str());
	}
	else {
		std::cerr << "Logger couldn't be initialized" << std::endl;
		exit(2);
	}
	return 0;
}