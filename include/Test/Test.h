#pragma once

#include "global.h"

struct TestResult {
	std::string testName;
	int testsSucceeded = 0;
	int testsTotal = 0;
};

/// Superclass for runnable tests
class Test {
public:
	virtual ~Test() {}

	/// returns whether test was successful
	virtual TestResult runTest() = 0;
};