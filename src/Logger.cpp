#include "Logger.h"

using namespace std;

Logger* g_logger;

void Logger::log(LogLevel level, const std::string& source, const std::string& message)
{
	if (level <= m_logLevel && level != LogLevel::None)
	{
		string levelString;
		switch (level)
		{
		case LogLevel::Debug:
			levelString = "[DEBUG]";
			break;
		case LogLevel::Info:
			levelString = "[INFO]";
			break;
		case LogLevel::Warning:
			levelString = "[WARNING]";
			break;
		case LogLevel::Error:
			levelString = "[ERROR]";
			break;
		case LogLevel::Verbose:
			levelString = "[VERBOSE]";
			break;
		default:
			break;
		}

		cout << levelString << "-[" << source << "]: " << message << endl;
	}
	
}
void Logger::logError(const std::string& source, const std::string& message)
{
	log(LogLevel::Error, source, message);
}
void Logger::logWarning(const std::string& source, const std::string& message)
{
	log(LogLevel::Warning, source, message);
}
void Logger::logInfo(const std::string& source, const std::string& message)
{
	log(LogLevel::Info, source, message);
}

void Logger::setLogLevel(LogLevel level)
{
	m_logLevel = level;
}

