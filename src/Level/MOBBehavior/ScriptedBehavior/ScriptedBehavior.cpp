#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "CharacterCore.h"
#include "Level/Enemy.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "Screens/Screen.h"

using namespace std;
using namespace luabridge;

const sf::Time ScriptedBehavior::SCRIPT_UPDATE_INTERVAL = sf::seconds(0.5f);

ScriptedBehavior::ScriptedBehavior(const std::string& luaPath, Enemy* enemy) : 
	m_callback(luaPath, enemy) {
	m_luaPath = luaPath;

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
	if (m_routineSteps.empty()) {
		// update callback
		updateTime(m_scriptUpdateTime, frameTime);
		if (m_scriptUpdateTime == sf::Time::Zero) {
			m_scriptUpdateTime = SCRIPT_UPDATE_INTERVAL;
			m_callback.update();
		}
	}
	else {
		// routine steps
		const RoutineStep& step = m_routineSteps.at(m_currentRoutineStep);
		switch (step.state) {
		case RoutineState::GoingTo:
			if (dist(m_enemy->getCenter(), step.goal) < Epsilon) {
				m_currentRoutineStep = (m_currentRoutineStep + 1) % m_routineSteps.size();
				setCurrentRoutineStep();
			}
			break;
		case RoutineState::Waiting:
			updateTime(m_waitingTime, frameTime);
			if (m_waitingTime == sf::Time::Zero) {
				m_currentRoutineStep = (m_currentRoutineStep + 1) % m_routineSteps.size();
				setCurrentRoutineStep();
			}
			break;
		default:
			m_currentRoutineStep = (m_currentRoutineStep + 1) % m_routineSteps.size();
			setCurrentRoutineStep();
			break;
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

void ScriptedBehavior::setCurrentRoutineStep() {
	m_enemy->resetMovingTarget();
	
	const RoutineStep& step = m_routineSteps.at(m_currentRoutineStep);
	switch (step.state) {
	case RoutineState::GoingTo:
		m_enemy->setMovingTarget(
			static_cast<int>(step.goal.x),
			static_cast<int>(step.goal.y));
		break;
	case RoutineState::Waiting:
		m_waitingTime = step.time;
		
		break;
	case RoutineState::Saying:
		m_speechBubble->setText(step.text);
		m_speechBubble->show();
		m_speechBubbleTime = step.time;
		break;
	case RoutineState::Animation:
		m_enemy->executeDefaultFightAnimation(true);
		break;
	case RoutineState::FacingDirection:
		m_enemy->setFacingRight(step.goal.x > 0);
		break;
	case RoutineState::Disappearing:
		m_enemy->notifyKilled();
		m_enemy->notifyLooted();
		break;
	case RoutineState::Spell: {
		int spell = std::atoi(step.text.c_str());
		m_enemy->executeSpell(spell, step.goal);
		break;
	}
	case RoutineState::SwitchLever: 
		m_enemy->switchLever();
		break;
	default:
		break;
	}
}

void ScriptedBehavior::addRoutineStep(const RoutineStep& step) {
	m_routineSteps.push_back(step);
	if (m_currentRoutineStep == std::string::npos) {
		m_currentRoutineStep = 0;
		setCurrentRoutineStep();
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