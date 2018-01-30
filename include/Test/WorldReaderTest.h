#pragma once

#include "global.h"
#include "Test/Test.h"

/// Tests whether all world files are fine and can be read.
class WorldReaderTest final : public Test {
public:
	TestResult runTest() override;

private:
	void loadMapFiles(TestResult& result);
	void loadLevelFiles(TestResult& result);

	template<typename R, typename D>
	void loadWorldFiles(TestResult& result, const std::string& type);
};