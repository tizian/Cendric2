#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "CharacterCore.h"
#include "Level/Enemy.h"
#include "Screen.h"

using namespace std;
using namespace luabridge;

const sf::Time ScriptedBehavior::SCRIPT_UPDATE_INTERVAL = sf::seconds(0.5f);

ScriptedBehavior::ScriptedBehavior(const std::string& luaPath, CharacterCore* core, Enemy* enemy) : 
	m_callback(luaPath, core, enemy) {

	m_callback.setScriptedBehavior(this);

	m_speechBubble = new SpeechBubble(enemy);
	m_speechBubble->hide();

	enemy->getScreen()->addObject(m_speechBubble);
}

ScriptedBehavior::~ScriptedBehavior() {
	m_speechBubble->setDisposed();
}

void ScriptedBehavior::update(const sf::Time& frameTime) {
	// update callback
	GameObject::updateTime(m_scriptUpdateTime, frameTime);
	if (m_scriptUpdateTime == sf::Time::Zero) {
		m_scriptUpdateTime = SCRIPT_UPDATE_INTERVAL;
		m_callback.update();
	}

	// update speech bubble
	if (m_speechBubbleTime > sf::Time::Zero) {
		GameObject::updateTime(m_speechBubbleTime, frameTime);
		if (m_speechBubbleTime == sf::Time::Zero) {
			m_speechBubble->hide();
		}
	}
}

void ScriptedBehavior::say(const std::string& text, int seconds) {
	m_speechBubble->setText(text);
	m_speechBubble->show();
	m_speechBubbleTime = sf::seconds(static_cast<float>(seconds));
}

void ScriptedBehavior::wait(int seconds) {
	m_scriptUpdateTime = sf::seconds(static_cast<float>(seconds));
}

bool ScriptedBehavior::isError() const {
	return !m_callback.isLoaded();
}

void ScriptedBehavior::setSpeechBubbleFloatingHeight(float height) {
	m_speechBubble->setFloatingHeight(height);
}
