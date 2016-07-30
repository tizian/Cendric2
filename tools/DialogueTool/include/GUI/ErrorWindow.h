#pragma once

#include "global.h"

// The error window shows the latest error.
class ErrorWindow {

public:
	ErrorWindow();
	void update();

	void addError(const std::string& errorMessage);

private:
	std::string m_errors;
};