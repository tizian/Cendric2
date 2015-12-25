#pragma once

#include "global.h"
#include "Map/NPCRoutine.h"

#include "LuaBridge/LuaBridge.h"

// helper class to load lua files for npc routines
class NPCRoutineLoader {
public:
	NPCRoutineLoader(NPCRoutine& routine);
	~NPCRoutineLoader();
	void loadRoutine();

	// methods to call in lua script
	void wait(int milliseconds);
	void goToTile(float x, float y);
	void setLooping(bool looping);

private:
	NPCRoutine& m_routine;
};