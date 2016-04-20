#include "Map/NPCRoutine.h"
#include "FileIO/NPCRoutineLoader.h"
#include "Map/NPC.h"
#include "Screens/Screen.h"

void NPCRoutine::load(const std::string& id, NPC* npc, bool initial) {
	m_id = id;
	m_npc = npc;
	m_steps.clear();
	m_currentStepID = 0;

	NPCRoutineLoader loader(*this, npc->getScreen()->getCharacterCore());
	loader.loadRoutine(initial);
	if (m_steps.empty()) {
		g_logger->logInfo("NPCRoutine", "This NPC has no routine!");
	}
	else {
		if (m_steps[0].state == RoutineState::Waiting) {
			m_remainingStepTime = m_steps[0].time;
		}
	}

	m_npc->setVelocity(sf::Vector2f(0.f, 0.f));
}

const std::string& NPCRoutine::getID() const {
	return m_id;
}

void NPCRoutine::update(const sf::Time& frameTime) {
	if (m_steps.empty()) return;
	bool updateStep = false;

	RoutineStep& currentStep = m_steps[m_currentStepID];
	if (currentStep.state == RoutineState::Disappearing) {
		m_npc->setDisposed();
		return;
	}
	else if (currentStep.state == RoutineState::Waiting) {
		m_remainingStepTime -= frameTime;
		if (m_remainingStepTime <= sf::Time::Zero) {
			updateStep = true;
		}
	}
	else if (currentStep.state == RoutineState::GoingTo) {
		sf::Vector2f distance = currentStep.goal - m_npc->getPosition();
		if (norm(distance) < 1.f) {
			updateStep = true;
		}
		else {
			sf::Vector2f newVel(0.f, 0.f);
			if (std::abs(distance.x) > 1.f) {
				newVel.x = (distance.x > 0.f) ? m_velocity : -m_velocity;
			}
			else if (std::abs(distance.y) > 1.f) {
				newVel.y = (distance.y > 0.f) ? m_velocity : -m_velocity;
			}
			else {
				newVel.x = (distance.x > 0.f) ? m_velocity : -m_velocity;
				newVel.y = (distance.y > 0.f) ? m_velocity : -m_velocity;
			}
			m_npc->setVelocity(newVel);
		}
	}

	if (updateStep) {
		m_currentStepID = (m_currentStepID >= static_cast<int>(m_steps.size()) - 1) ?
			0 : 
			m_currentStepID + 1;

		if (m_currentStepID == 0 && !m_isLooped) {
			m_steps.clear();
			m_npc->setVelocity(sf::Vector2f(0.f, 0.f));
			return;
		}

		if (m_steps[m_currentStepID].state == RoutineState::Waiting) {
			m_remainingStepTime = m_steps[m_currentStepID].time;
			m_npc->setVelocity(sf::Vector2f(0.f, 0.f));
		}
	}
}

void NPCRoutine::addStep(const RoutineStep& step) {
	m_steps.push_back(step);
}

void NPCRoutine::setLooped(bool looped) {
	m_isLooped = looped;
}

void NPCRoutine::setVelocity(float velocity) {
	m_velocity = velocity;
}

NPC* NPCRoutine::getNPC() const {
	return m_npc;
}