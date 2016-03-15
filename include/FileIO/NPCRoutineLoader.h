#pragma once

#include "global.h"
#include "Map/NPCRoutine.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// helper class to load lua files for npc routines
class NPCRoutineLoader {
public:
	NPCRoutineLoader(NPCRoutine& routine, CharacterCore* core);
	~NPCRoutineLoader();
	void loadRoutine(bool isInitial);

	// methods to call in lua script
	bool isConditionFulfilled(const std::string& conditionType, const std::string& condition) const;
	bool isQuestState(const std::string& questID, const std::string& state) const;
	void setTilePosition(float x, float y);
	void setDisposed();
	void wait(int milliseconds);
	void goToTile(float x, float y);
	void setLooped(bool looped);
	void setTalkingActive(bool active);
	void setTalkingEnabled(bool enabled);

private:
	bool m_isInitial = true;
	NPCRoutine& m_routine;
	CharacterCore* m_core;
};