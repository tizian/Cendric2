#pragma once

#include "global.h"
#include "Test/Test.h"

/// Tests whether all dialogue keys are translated.
class DialogueTranslationTest final : public Test {
public:
	TestResult runTest() override;

private:
	void loadDialogueFiles(TestResult& result);
};