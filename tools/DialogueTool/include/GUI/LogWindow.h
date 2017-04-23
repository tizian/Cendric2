#pragma once

#include "global.h"

// The log window shows the latest log output.
class LogWindow final {

public:
	LogWindow();
	void update();

	void addLog(const std::string& logMessage, const sf::Color& color);

private:
	std::vector<std::pair<std::string, sf::Color>> m_logs;
};