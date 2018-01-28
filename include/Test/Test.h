#pragma once

#include "global.h"

/// Superclass for runnable tests
class Test {
public:
	virtual ~Test() {}

	/// returns whether test was successful
	virtual bool runTest() = 0;
};