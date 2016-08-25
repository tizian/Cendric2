#pragma once

#include "global.h"

#include "LuaBridge/LuaBridge.h"

class Enemy;
class ScriptedBehavior;
class WorldCallback;

class ScriptedBehaviorCallback {
public:
	ScriptedBehaviorCallback(const std::string& luaPath, Enemy* enemy);
	~ScriptedBehaviorCallback();

	void update();
	void onDeath();

	void setScriptedBehavior(ScriptedBehavior* behavior);

	// methods for questions about the enemy's position
	int getPosX() const;
	int getPosY() const;

	// Display time of the speech bubble in seconds.
	// in update function: immediately displayed (unless there's wait called before)
	// in routine function: adds a talking step
	void say(const std::string& text, int seconds);
	// in update function: wait for a certain time before the next update is called
	// in routine function: add a waiting step
	void wait(int seconds);
	// the enemy tries to move to this target as long as it is not reset.
	// can only be used in update functions
	void setMovingTarget(int x, int y);
	// adds a goto step in the routine. Can only be used in the routine function
	void gotoTile(float x, float y);
	// resets the moving target
	void resetMovingTarget();
	// sets the enemy "killed" and "looted" so that it will never appear again in this level
	// can only be used in update function (for now)
	void setDisposed();
	// leaves the level
	//  can only be used in update function (for now)
	void leaveLevel();
	// executes a blocking fight animation (uses the default fight animation) for that mob.
	void executeFightAnimation();
	// adds a hint to the screen
	// can only be used in update function(for now)
	void addHint(const std::string& hint);
	bool isLoaded() const;

private:
	Enemy* m_enemy;
	ScriptedBehavior* m_scriptedBehavior;
	WorldCallback* m_worldCallback;
	luabridge::lua_State* m_L;

	// return whether it was successful in loading or not
	// it also sets the observer steps in the scripted behavior if it finds that function
	bool loadLua(const std::string& path, ScriptedBehavior* behavior);
	std::string m_luaPath;
	bool m_success = false;

	bool m_hasUpdateFunc = false;
	bool m_hasDeathFunc = false;
	bool m_isRoutineFunction = false;
};