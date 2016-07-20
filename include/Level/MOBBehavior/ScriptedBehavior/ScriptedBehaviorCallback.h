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

	// method for talking. Display time of the speech bubble in seconds.
	void say(const std::string& text, int seconds);
	// wait for a certain time before the next update is called
	void wait(int seconds);
	// the enemy tries to move to this target as long as it is not reset.
	void setMovingTarget(int x, int y);
	// resets the moving target
	void resetMovingTarget();
	// sets the enemy "killed" and "looted" so that it will never appear again in this level
	void setDisposed();
	// leaves the level
	void leaveLevel();
	// adds a hint to the screen
	void addHint(const std::string& hint);
	// sets a certain condition fulfilled
	void addConditionProgress(const std::string& conditionType, const std::string& condition);

	bool isLoaded() const;

private:
	Enemy* m_enemy;
	ScriptedBehavior* m_scriptedBehavior;
	WorldCallback* m_worldCallback;
	luabridge::lua_State* m_L;

	// return whether it was successful in loading or not
	bool loadLua(const std::string& path);
	bool m_success = false;

	bool m_hasUpdateFunc = false;
	bool m_hasDeathFunc = false;
};