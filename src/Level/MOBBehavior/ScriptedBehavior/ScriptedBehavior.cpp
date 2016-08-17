#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "CharacterCore.h"
#include "Level/Enemy.h"
#include "Screens/Screen.h"

using namespace std;
using namespace luabridge;

const sf::Time ScriptedBehavior::SCRIPT_UPDATE_INTERVAL = sf::seconds(0.5f);

ScriptedBehavior::ScriptedBehavior(const std::string& luaPath, Enemy* enemy) : 
	m_callback(luaPath, enemy) {

	m_speechBubble = new SpeechBubble(enemy);
	m_speechBubble->setFloatingHeight(enemy->getConfiguredDistanceToHPBar() + 10.f);
	m_speechBubble->hide();

	enemy->getScreen()->addObject(m_speechBubble);
	m_enemy = enemy;

	m_callback.setScriptedBehavior(this);
}

ScriptedBehavior::~ScriptedBehavior() {
	m_speechBubble->setDisposed();
}

void ScriptedBehavior::update(const sf::Time& frameTime) {
	if (m_observerSteps.empty()) {
		// update callback
		updateTime(m_scriptUpdateTime, frameTime);
		if (m_scriptUpdateTime == sf::Time::Zero) {
			m_scriptUpdateTime = SCRIPT_UPDATE_INTERVAL;
			m_callback.update();
		}
	}
	else {
		// observer steps
		if (dist(m_enemy->getCenter(), m_observerSteps.at(m_currentObserverStep)) < 10.f) {
			m_currentObserverStep = (m_currentObserverStep + 1) % m_observerSteps.size();
			setCurrentObserverStep();
		}
	}
	updateSpeechBubble(frameTime);
}

void ScriptedBehavior::updateSpeechBubble(const sf::Time& frameTime) {
	// update speech bubble
	if (m_speechBubbleTime > sf::Time::Zero) {
		updateTime(m_speechBubbleTime, frameTime);
		if (m_speechBubbleTime == sf::Time::Zero) {
			m_speechBubble->hide();
		}
	}
}

void ScriptedBehavior::setCurrentObserverStep() {
	m_enemy->setMovingTarget(
		static_cast<int>(m_observerSteps.at(m_currentObserverStep).x),
		static_cast<int>(m_observerSteps.at(m_currentObserverStep).y));
}

void ScriptedBehavior::addObserverStep(float x, float y) { 
	m_observerSteps.push_back(sf::Vector2f(x, y));
	if (m_currentObserverStep == std::string::npos) {
		m_currentObserverStep = 0;
		setCurrentObserverStep();
	}
}

void ScriptedBehavior::onDeath() {
	m_callback.onDeath();
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