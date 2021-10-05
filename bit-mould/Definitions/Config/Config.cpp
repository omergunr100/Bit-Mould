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
int temp = Config::Parse();