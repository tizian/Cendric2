#pragma once

#include "global.h"
#include "Logger.h"
#include "Structs/CutsceneData.h"

#include "LuaBridge/LuaBridge.h"

// static helper class to load lua files for cutscenes
class CutsceneLoader {
public:
	static CutsceneData loadCutscene(const std::string& cutsceneID);

private:
	static const std::string CUTSCENE_FOLDER;
};