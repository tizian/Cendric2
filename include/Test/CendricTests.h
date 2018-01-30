#pragma once

#include "global.h"

/// Static class for testing
class CendricTests final {
private:
	CendricTests() {};

public:
	static void runTests();

private:
	template<typename T>
	static void runTest();
};