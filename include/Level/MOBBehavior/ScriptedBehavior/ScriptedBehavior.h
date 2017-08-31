#pragma once

#include "global.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehaviorCallback.h"
#include "GUI/SpeechBubble.h"
#include "Structs/RoutineStep.h"

class CharacterCore;
class Enemy;

// a scripted behavior with an update function that uses a lua callback.
// used for enemies with a special behavior, like talking
class ScriptedBehavior final {
public:
	ScriptedBehavior(const std::string& luaPath, Enemy* enemy);
	~ScriptedBehavior();

	void update(const sf::Time& frameTime);
	void updateSpeechBubble(const sf::Time& frameTime);
	void onDeath();
	void say(const std::string& text, int seconds);
	void wait(int seconds);
	void addRoutineStep(const RoutineStep& step);
	void setCurrentRoutineStep();

	bool isError() const;
	const std::string& getLuaPath() const { return m_luaPath; }

protected:
	std::string m_luaPath;
	Enemy* m_enemy;
	SpeechBubble* m_speechBubble;
	ScriptedBehaviorCallback m_callback;
	sf::Time m_speechBubbleTime = sf::Time::Zero;
	sf::Time m_scriptUpdateTime = sf::Time::Zero;

	static const sf::Time SCRIPT_UPDATE_INTERVAL;

	// if this is filled, the behavior will only do these steps
	std::vector<RoutineStep> m_routineSteps;
	size_t m_currentRoutineStep = std::string::npos;
	sf::Time m_waitingTime = sf::Time::Zero;
};