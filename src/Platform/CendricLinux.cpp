#include "Platform/CendricLinux.h"

#ifdef __linux__

#include <cstdlib>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include "Logger.h"

std::string getExternalDocumentsPath() {
	std::string resultPath = "";
	std::string savePath = "";
	if(const char* env_p = std::getenv("XDG_DATA_HOME"))
		resultPath = std::string(env_p) + "/Cendric/";
	else if(const char* env_p = std::getenv("HOME"))
        	resultPath = std::string(env_p) + "/.local/share/Cendric/";
	
	savePath = resultPath + std::string("saves");
	if(mkdir(resultPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		int errorpc = errno;
		if(errorpc != EEXIST) {
			g_logger->logError("PlatformLinux", "Dir " + resultPath + " could not be created. Error: " + std::to_string(errorpc));
		}
	}
	if(mkdir(savePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		int errorpc = errno;
		if(errorpc != EEXIST) {
			g_logger->logError("PlatformLinux", "Dir " + savePath + " could not be created. Error: " + std::to_string(errorpc));
		}
	}

	return resultPath;
}

std::string getSystemResourcePath() {
	return RESDIR;
}

#endif
