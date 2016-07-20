#pragma once

#include "global.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehaviorCallback.h"
#include "GUI/SpeechBubble.h"

class CharacterCore;
class Enemy;

// a scripted behavior with an update function that uses a lua callback.
// used for enemies with a special behavior, like talking
class ScriptedBehavior {
public:
	ScriptedBehavior(const std::string& luaPath, Enemy* enemy);
	~ScriptedBehavior();

	void update(const sf::Time& frameTime);
	void onDeath();
	void say(const std::string& text, int seconds);
	void wait(int seconds);

	bool isError() const;

protected:
	SpeechBubble* m_speechBubble;
	ScriptedBehaviorCallback m_callback;
	sf::Time m_speechBubbleTime = sf::Time::Zero;
	sf::Time m_scriptUpdateTime = sf::Time::Zero;

	static const sf::Time SCRIPT_UPDATE_INTERVAL;
};