#include "Cutscene/CutsceneLoader.h"

using namespace std;
using namespace luabridge;

const std::string CutsceneLoader::CUTSCENE_FOLDER = "res/cutscene/";

CutsceneData CutsceneLoader::loadCutscene(const std::string& cutsceneID) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	CutsceneData cutsceneData;
	cutsceneData.id = "";

	std::string foldername = CUTSCENE_FOLDER + cutsceneID + "/";
	std::string filename = foldername + cutsceneID + ".lua";

	if (luaL_dofile(L, filename.c_str()) != 0) {
		g_logger->logError("CutsceneLoader", "Cannot read lua script: " + filename);
		return cutsceneData;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef musicpath = getGlobal(L, "musicpath");
	if (musicpath.isString()) {
		cutsceneData.musicPath = musicpath.cast<std::string>();
	}

	LuaRef levelid = getGlobal(L, "levelid");
	if (levelid.isString()) {
		cutsceneData.levelID = levelid.cast<std::string>();
	}

	LuaRef mapid = getGlobal(L, "mapid");
	if (mapid.isString()) {
		cutsceneData.mapID = mapid.cast<std::string>();
	}

	LuaRef worldy = getGlobal(L, "worldy");
	if (worldy.isNumber()) {
		cutsceneData.wordPosition.y = worldy.cast<float>();
	}

	LuaRef worldx = getGlobal(L, "worldx");
	if (worldx.isNumber()) {
		cutsceneData.wordPosition.x = worldx.cast<float>();
	}

	LuaRef steps = getGlobal(L, "steps");
	if (steps.isTable()) {
		int i = 1; // in lua, the first element is 1, not 0. Like Eiffel haha.
		LuaRef step = steps[i];
		while (!step.isNil()) {
			LuaRef texts = step[1];
			LuaRef images = step[2];
			if (!texts.isTable() || !images.isTable()) {
				g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: step table not resolved, there must be one table for texts and one for images.");
				return cutsceneData;
			}

			CutsceneStep cutsceneStep;

			// resolve texts
			int j = 1;
			LuaRef text = texts[j];
			while (!text.isNil()) {
				LuaRef textString = text[1];
				LuaRef textTime = text[2];

				if (!textString.isString() || !textTime.isNumber()) {
					g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: text table not resolved, text string must be of type string and text time of type number.");
					return cutsceneData;
				}

				CutsceneText cutsceneText;
				cutsceneText.text = textString.cast<std::string>();
				cutsceneText.time = sf::seconds(static_cast<float>(textTime.cast<int>()));

				cutsceneStep.texts.push_back(cutsceneText);

				j++;
				text = texts[j];
			}

			// resolve images
			j = 1;
			LuaRef image = images[j];
			while (!image.isNil()) {
				LuaRef imagePath = image[1];
				LuaRef velocity = image[2];
				LuaRef angle = image[3];

				if (!imagePath.isString() || !velocity.isNumber() || !angle.isNumber()) {
					g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: image table not resolved, image path must be of type string and velocity and angle of type number.");
					return cutsceneData;
				}

				float phi = degToRad(static_cast<float>(angle.cast<int>() - 90));
				float speed = static_cast<float>(velocity.cast<int>());

				CutsceneImage cutsceneImage;
				cutsceneImage.imagePath = foldername + imagePath.cast<std::string>();
				cutsceneImage.velocity.x = std::round(speed * std::cos(phi));
				cutsceneImage.velocity.y = std::round(speed * std::sin(phi));

				cutsceneStep.images.push_back(cutsceneImage);

				j++;
				image = images[j];
			}

			cutsceneData.steps.push_back(cutsceneStep);
			i++;
			step = steps[i];
		}
	}

	cutsceneData.id = cutsceneID;
	return cutsceneData;
}
