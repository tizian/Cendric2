#include "Cutscene/CutsceneLoader.h"
#include "ResourceManager.h"
#include "Enums/Language.h"

using namespace luabridge;

const std::string CutsceneLoader::CUTSCENE_FOLDER = "res/cutscene/";

CutsceneData CutsceneLoader::loadCutscene(const std::string& _cutsceneID) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	CutsceneData cutsceneData;
	cutsceneData.id = "";

	std::string cutsceneID;
	std::string cutsceneSubfolder;
	size_t slashPos = _cutsceneID.find("/");
	if (slashPos != std::string::npos) {
		cutsceneSubfolder = _cutsceneID.substr(0, slashPos);
		cutsceneID = _cutsceneID.substr(slashPos + 1);
	}
	else {
		cutsceneID = _cutsceneID;
		cutsceneSubfolder = _cutsceneID;
	}

	std::string foldername = CUTSCENE_FOLDER + cutsceneSubfolder + "/";
	std::string filename = getResourcePath(foldername + cutsceneID + ".lua");

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
			LuaRef texts = step["texts"];
			LuaRef images = step["images"];
			if (!texts.isTable() || !images.isTable()) {
				g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: step table not resolved, there must be one table for texts and one for images.");
				return cutsceneData;
			}

			CutsceneStep cutsceneStep;

			// resolve texts
			int j = 1;
			LuaRef text = texts[j];
			while (!text.isNil()) {
				LuaRef textString = text["str"];
				LuaRef textTime = text["time"];

				if (!textString.isString() || !textTime.isNumber()) {
					g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: text table not resolved, text string must be of type string and text time of type number.");
					return cutsceneData;
				}

				CutsceneText cutsceneText;
				cutsceneText.text = textString.cast<std::string>();
				cutsceneText.time = sf::seconds(static_cast<float>(textTime.cast<int>()));
				cutsceneText.centered = false;

				LuaRef centered = text["centered"];
				if (!centered.isNil()) {
					cutsceneText.centered = true;
				}

				cutsceneStep.texts.push_back(cutsceneText);

				++j;
				text = texts[j];
			}

			// resolve images
			j = 1;
			LuaRef image = images[j];
			while (!image.isNil()) {
				LuaRef imagePath = image["path"];
				LuaRef velocity = image["vel"];
				LuaRef angle = image["angle"];

				if (!(imagePath.isString() || imagePath.isTable()) || !velocity.isNumber() || !angle.isNumber()) {
					g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: image table not resolved, image path must be of type string or table and velocity and angle of type number.");
					return cutsceneData;
				}

				std::string imagePathStr;
				if (imagePath.isString()) {
					imagePathStr = imagePath.cast<std::string>();
				}
				else if (imagePath.isTable()) {

					Language language = g_resourceManager->getConfiguration().language;
					std::string key;
					if (language == Language::Lang_EN) {
						key = "en";
					}
					else if (language == Language::Lang_DE) {
						key = "de";
					}
					else if (language == Language::Lang_CH) {
						key = "ch";
					}
					else if (language == Language::LANG_ES) {
						key = "es";
					}

					LuaRef imagePathLang = imagePath[key];

					if (!imagePathLang.isString()) {
						g_logger->logError("CutsceneLoader", "Cutscene [" + filename + "]: image table not resolved, use a table of exactly 3 strings \"en\", \"de\" and \"ch\" to specify language dependent images.");
						return cutsceneData;
					}

					imagePathStr = imagePathLang.cast<std::string>();
				}

				float phi = degToRad(static_cast<float>(angle.cast<int>() - 90));
				float speed = static_cast<float>(velocity.cast<int>());

				CutsceneImage cutsceneImage;
				cutsceneImage.imagePath = foldername + imagePathStr;
				cutsceneImage.velocity.x = std::round(speed * std::cos(phi));
				cutsceneImage.velocity.y = std::round(speed * std::sin(phi));

				cutsceneStep.images.push_back(cutsceneImage);

				++j;
				image = images[j];
			}

			// fadetime
			LuaRef fadeTime = step["fadetime"];
			if (fadeTime.isNumber()) {
				cutsceneStep.fadeTime = sf::seconds(static_cast<float>(fadeTime.cast<float>()));
			}
			else {
				cutsceneStep.fadeTime = sf::Time::Zero;
			}

			cutsceneData.steps.push_back(cutsceneStep);
			++i;
			step = steps[i];
		}
	}

	cutsceneData.id = cutsceneID;
	return cutsceneData;
}
