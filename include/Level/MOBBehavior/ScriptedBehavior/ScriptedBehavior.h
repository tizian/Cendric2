#pragma once

#include "global.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// a scripted behavior with an update function that uses a lua callback.
// used for enemies with a special behavior, like talking
class ScriptedBehavior {
public:
	ScriptedBehavior(const std::string& luaPath, CharacterCore* core);
	~ScriptedBehavior();

	// methods for questions about the current game (character core) state
	bool isQuestState(const std::string& questID, const std::string& state) const;
	bool isQuestComplete(const std::string& questID);
	bool isConditionFulfilled(const std::string& condition) const;

	// methods for questions about the enemy's position
	int getPosX() const;
	int getPosY() const;

	// method for talking. If the milliseconds parameter is set to 0, 
	// the speech bubble will only disappear when another "say" is called
	void say(const std::string& text, int seconds);

	// sets the enemy "looted" so that it will never appear again in this level
	void setLooted();
	// leaves the level
	void leaveLevel();

private:
	CharacterCore* m_core;
};