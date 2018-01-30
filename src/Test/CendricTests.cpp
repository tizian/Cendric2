#include "Test/CendricTests.h"
#include "Test/WorldReaderTest.h"
#include "Test/DialogueTranslationTest.h"
#include "Logger.h"

void CendricTests::runTests() {
	runTest<WorldReaderTest>();
	runTest<DialogueTranslationTest>();
}

template<typename T>
static void CendricTests::runTest() {
	T wlTest;
	TestResult tr = wlTest.runTest();

	g_logger->logWarning("CendricTests", tr.testName + ": " +
		std::to_string(tr.testsSucceeded) + " / " + std::to_string(tr.testsSucceeded) +
		" tests passed.");
}