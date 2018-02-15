#pragma once

// the importance of the log output. 0 is the highest (None) 
// and should never be used to log messages (would have no effect)
enum class LogLevel {
	None,
	Error,
	Warning,
	Info,
	Debug,
	Verbose,
};