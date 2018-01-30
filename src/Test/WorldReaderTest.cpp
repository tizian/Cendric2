#include "Test/WorldReaderTest.h"
#include "ResourceManager.h"

#include "FileIO/LevelReader.h"
#include "FileIO/MapReader.h"
#include "CharacterCore.h"

#ifdef _WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

inline bool ends_with(const std::string& value, const std::string& ending) {
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

TestResult WorldReaderTest::runTest() {
	TestResult result;
	result.testName = "WorldReaderTest";

	loadMapFiles(result);
	loadLevelFiles(result);

	return result;
}

void WorldReaderTest::loadMapFiles(TestResult& result) {
	loadWorldFiles<MapReader, MapData>(result, "map");
}

void WorldReaderTest::loadLevelFiles(TestResult& result) {
	loadWorldFiles<LevelReader, LevelData>(result, "level");
}

template<typename R, typename D>
void WorldReaderTest::loadWorldFiles(TestResult& result, const std::string& type) {
	DIR* dir;
	DIR* innerDir;
	struct dirent* de;
	struct dirent* innerDe;

	auto basepath = "res/" + type;

	dir = opendir(basepath.c_str());

	CharacterCore* core = new CharacterCore();
	core->loadNew();

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

				if (!ends_with(std::string(innerDe->d_name), ".tmx")) {
					continue;
				}

				auto worldPath = innerDirPath + "/" + std::string(innerDe->d_name);

				R* reader = new R();
				D data;
				g_logger->logInfo("[WorldReaderTest]", "Reading world: " + worldPath);
				bool noError = reader->readWorld(worldPath, data, core);
				delete reader;

				result.testsTotal++;
				if (noError) {
					result.testsSucceeded++;
					g_logger->logInfo("[WorldReaderTest]", "World succeeded: " + worldPath);
				}
				else {
					g_logger->logError("[WorldReaderTest]", "World corrupted: " + worldPath);
				}
			}

			closedir(innerDir);
		}
	}
	closedir(dir);

	delete core;
}