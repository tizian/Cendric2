#include "Test/DialogueTranslationTest.h"
#include "TextProvider.h"
#include "ResourceManager.h"
#include <string>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

inline bool ends_with(const std::string& value, const std::string& ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline bool starts_with(const std::string& value, const std::string& start) {
	if (start.size() > value.size()) return false;
	return std::equal(start.begin(), start.end(), value.begin());
}

TestResult DialogueTranslationTest::runTest() {
	TestResult result;
	result.testName = "DialogueTranslationTest";

	loadDialogueFiles(result);

	return result;
}

void DialogueTranslationTest::loadDialogueFiles(TestResult& result) {
	DIR* dir;
	DIR* innerDir;
	struct dirent* de;
	struct dirent* innerDe;

	std::string basepath = "res/npc";

	Language oldLang = g_resourceManager->getConfiguration().language;

	dir = opendir(basepath.c_str());

	while (dir) {
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			if (de->d_name[0] == '.') continue;

			auto innerDirPath = basepath + "/" + std::string(de->d_name);

			innerDir = opendir(innerDirPath.c_str());

			while (innerDir) {
				innerDe = readdir(innerDir);
				if (!innerDe) break;
				if (innerDe->d_type == DT_DIR) {
					continue;
				}

				if (!starts_with(std::string(innerDe->d_name), "dl_") || !ends_with(std::string(innerDe->d_name), ".lua")) {
					continue;
				}

				auto filePath = innerDirPath + "/" + std::string(innerDe->d_name);
				std::string textType = std::string(innerDe->d_name);
				textType = textType.substr(0, textType.size() - 4); // remove ".lua"

				std::ifstream file(filePath);
				std::stringstream buffer;
				buffer << file.rdbuf();
				std::string content = buffer.str();
				size_t pos = 1;

				while (true) {
					pos = content.find("DL_");

					if (pos == std::string::npos) {
						break;
					}

					content = content.substr(pos);

					pos = content.find("\"");

					if (pos == std::string::npos) {
						break;
					}

					std::string textKey = content.substr(0, pos);

					content = content.substr(pos);

					g_textProvider->setLanguage(Language::Lang_EN);

					if (g_textProvider->isTextTranslated(textKey, textType)) {
						result.testsSucceeded++;
						g_logger->logInfo("[DialogueTranslationTest]", "EN Dialogue translations succeeded: " + filePath);
					}
					else {
						g_logger->logError("[DialogueTranslationTest]", "EN Dialogue translations missing in: " + filePath);
					}
					result.testsTotal++;

					g_textProvider->setLanguage(Language::Lang_DE);

					if (g_textProvider->isTextTranslated(textKey, textType)) {
						result.testsSucceeded++;
						g_logger->logInfo("[DialogueTranslationTest]", "DE Dialogue translations succeeded: " + filePath);
					}
					else {
						g_logger->logError("[DialogueTranslationTest]", "DE Dialogue translations missing in: " + filePath);
					}
					result.testsTotal++;

					g_textProvider->setLanguage(Language::Lang_CH);

					if (g_textProvider->isTextTranslated(textKey, textType)) {
						result.testsSucceeded++;
						g_logger->logInfo("[DialogueTranslationTest]", "CH Dialogue translations succeeded: " + filePath);
					}
					else {
						g_logger->logError("[DialogueTranslationTest]", "CH Dialogue translations missing in: " + filePath);
					}
					result.testsTotal++;
				}
			}

			closedir(innerDir);
		}
	}
	closedir(dir);

	g_textProvider->setLanguage(oldLang);
}