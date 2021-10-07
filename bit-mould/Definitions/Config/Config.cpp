#include "Config.h"

const char* configPath = "External Files/Config.txt";
std::map<std::string, std::string> Config::config;

int Config::Parse()
{
	std::ifstream config(configPath);
	if (config.good()) {
		std::string line;
		while (std::getline(config, line)) {
			if (line.find('=')!=std::string::npos) {
				std::istringstream reLine(line);
				std::string incompleteKey;
				if (std::getline(reLine, incompleteKey, '=')) {
					std::istringstream reKey(incompleteKey);
					std::string key, value;
					std::getline(reLine, value);
					std::getline(reKey, key, ':');
					Config::config[key] = value;
				}
			}
		}
		config.close();
	}
	else {
		std::cerr << "[Fatal Error] Config file could not be opened!" << std::endl;
		exit(1); 
	}
	return 0;
}
const char * Config::GetAsChar(const char * id)
{
	return config.at(id).c_str();
}
std::string Config::GetAsString(const char * id)
{
	return config.at(id);
}
int Config::GetAsInteger(const char * id)
{
	return std::stoi(config.at(id));
}
float Config::GetAsFloat(const char * id)
{
	return std::stof(config.at(id));
}
bool Config::GetAsBool(const char * id)
{
	return config.at(id) == "true";
}
int temp = Config::Parse();