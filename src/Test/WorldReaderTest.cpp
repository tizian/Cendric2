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

bool WorldReaderTest::runTest() {
	if (!loadMapFiles()) {
		return false;
	}

	if (!loadLevelFiles()) {
		return false;
	}

	return true;
}

bool WorldReaderTest::loadMapFiles() {
	DIR* dir;
	struct dirent* de;

	dir = opendir("res/map");
	bool noError = true;
	
	CharacterCore* core = new CharacterCore();
	
	core->loadNew();
	
	while (dir && noError) {
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			if (de->d_name[0] == '.') continue;

			auto worldPath = "res/map/" + std::string(de->d_name) + "/" + std::string(de->d_name) + ".tmx";

			MapReader* reader = new MapReader();
			MapData data;
			g_logger->logInfo("[WorldReaderTest]", "Reading map: " + worldPath);
			noError = reader->readMap(worldPath, data, core);
			delete reader;

			if (!noError) {
				break;
			}
		}
	}
	closedir(dir);

	delete core;

	return noError;
}

bool WorldReaderTest::loadLevelFiles() {
	DIR* dir;
	DIR* innerDir;
	struct dirent* de;
	struct dirent* innerDe;

	dir = opendir("res/level");
	bool noError = true;

	CharacterCore* core = new CharacterCore();
	core->loadNew();

	while (dir && noError) {
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			if (de->d_name[0] == '.') continue;

			auto innerDirPath = "res/level/" + std::string(de->d_name);

			DIR* innerDir = opendir(innerDirPath.c_str());
			
			while (innerDir && noError) {
				innerDe = readdir(innerDir);
				if (!innerDe) break;
				if (innerDe->d_type == DT_DIR) {
					continue;
				}

				if (!ends_with(std::string(innerDe->d_name), ".tmx")) {
					continue;
				}

				auto worldPath = "res/level/" + std::string(de->d_name) + "/" + std::string(innerDe->d_name);

				LevelReader* reader = new LevelReader();
				LevelData data;
				g_logger->logInfo("[WorldReaderTest]", "Reading level: " + worldPath);
				noError = reader->readLevel(worldPath, data, core);
				delete reader;

				if (!noError) {
					break;
				}
			}

			closedir(innerDir);

			if (!noError) {
				break;
			}
		}
	}
	closedir(dir);

	delete core;

	return noError;
}