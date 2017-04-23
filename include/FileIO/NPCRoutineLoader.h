#pragma once

#include "global.h"
#include "Map/NPCRoutine.h"

#include "LuaBridge/LuaBridge.h"

class WorldCallback;
class WorldScreen;

// helper class to load lua files for npc routines
class NPCRoutineLoader final {
public:
	NPCRoutineLoader(NPCRoutine& routine, WorldScreen* screen);
	~NPCRoutineLoader();
	void loadRoutine(bool isInitial);

	// methods to call in lua script
	void setTilePosition(float x, float y);
	void setTilePositionForce(float x, float y);
	void setFacingUp();
	void setFacingDown();
	void setFacingRight();
	void setFacingLeft();
	void setDisposed();
	void setDisposedStep();
	void wait(int milliseconds);
	void goToTile(float x, float y);
	void setLooped(bool looped);
	void setTalkingActive(bool active);
	void setTalkingActiveForce(bool active);
	void setTalkingEnabled(bool enabled);
	void setReloadEnabled(bool enabled);

private:
	bool m_isInitial = true;
	NPCRoutine& m_routine;
	WorldCallback* m_worldCallback;
};