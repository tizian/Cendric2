#pragma once

#include "global.h"
#include "Test/Test.h"

/// Tests whether all world files are fine and can be read.
class WorldReaderTest final : public Test {
public:
	bool runTest() override;

private:
	bool loadMapFiles();
	bool loadLevelFiles();
};