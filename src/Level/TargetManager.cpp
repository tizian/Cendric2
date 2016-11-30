#include "Level/TargetManager.h"
#include "Level/LevelMainCharacter.h"
#include "Level/LevelInterface.h"
#include "Level/Enemy.h"
#include "Screens/LevelScreen.h"

const float TargetManager::TARGET_RANGE = 600.f;

TargetManager::TargetManager() {
}

TargetManager::~TargetManager() {
}

void TargetManager::setMainCharacter(LevelMainCharacter* mainChar) {
	m_mainChar = mainChar;
	m_screen = dynamic_cast<LevelScreen*>(m_mainChar->getScreen());
}

void TargetManager::update(const sf::Time& frameTime) {
	// Remove target if right-clicked anywhere
	if (g_inputController->isMouseClickedRight()) {
		setTargetEnemy(nullptr);
	}

	// enemy no longer targeted if it moves out of view
	if (m_targetedEnemy && !m_targetedEnemy->isViewable()) {
		setTargetEnemy(nullptr);
	}

	if (g_inputController->isKeyJustPressed(Key::SwitchTarget)) {
		// check which enemy is the next to target
		Enemy* currentNearest = nullptr;
		float currentDistance = TARGET_RANGE;
		for (auto& go : *m_screen->getObjects(GameObjectType::_Enemy)) {
			if (Enemy* enemy = dynamic_cast<Enemy*>(go)) {
				if (enemy->isAlly() || enemy->isDead() || enemy->isDisposed()) continue;
				float distance = dist(enemy->getCenter(), m_mainChar->getCenter());
				if (distance < currentDistance && 
					m_previousTargets.find(enemy) == m_previousTargets.end()) {
					currentNearest = enemy;
					currentDistance = distance;
				}
			}
		}
		setTargetEnemy(currentNearest);
	}
}

void TargetManager::setTargetEnemy(Enemy* enemy) {
	if (enemy && (enemy->isDead() || enemy->isDisposed())) {
		enemy = nullptr;
	}
	if (m_targetedEnemy) {
		m_targetedEnemy->setTargeted(false);
	}
	m_targetedEnemy = enemy;
	if (m_targetedEnemy) {
		m_targetedEnemy->setTargeted(true);
		m_previousTargets.insert(enemy);
	}
	else {
		m_previousTargets.clear();
	}
	m_screen->setEnemyForHealthBar(m_targetedEnemy);
}

Enemy* TargetManager::getCurrentTargetEnemy() const {
	return m_targetedEnemy;
}
