#pragma once

#include "global.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;
class Enemy;
class ScriptedBehavior;

class ScriptedBehaviorCallback {
public:
	ScriptedBehaviorCallback(const std::string& luaPath, CharacterCore* core, Enemy* enemy);
	~ScriptedBehaviorCallback();

	void update();

	void setScriptedBehavior(ScriptedBehavior* behavior);

	// methods for questions about the current game (character core) state
	bool isQuestState(const std::string& questID, const std::string& state) const;
	bool isQuestComplete(const std::string& questID) const;
	bool isConditionFulfilled(const std::string& condition) const;

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
	// sets the enemy "killed" so that it will never appear again in this level
	void setKilled();
	// leaves the level
	void leaveLevel();
	// sets a certain condition fulfilled
	void addConditionProgress(const std::string& condition);

	bool isLoaded() const;

private:
	CharacterCore* m_core;
	Enemy* m_enemy;
	ScriptedBehavior* m_scriptedBehavior;
	luabridge::lua_State* m_L;

	// return whether it was successful in loading or not
	bool loadLua(const std::string& path);
	bool m_success = false;
};