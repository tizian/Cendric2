#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Enums/EnemyAttitude.h"

// An enemy attacking behavior
class AttackingBehavior {
public:
	AttackingBehavior(Enemy* enemy);
	virtual ~AttackingBehavior() {};

	virtual void update(const sf::Time& frameTime);
	virtual void updateAggro() = 0;
	void handleAttackInput();

	void setAggroRange(float range);
	void setAttackInput(const std::function<void()>& agent);

	virtual EnemyAttitude getAttitude() const = 0;
	virtual sf::Color getConfiguredHealthColor() const = 0;
	const LevelMovableGameObject* getCurrentTarget() const;
	float getAggroRange() const;
	float distToTarget() const;

protected:
	Enemy* m_enemy;
	float m_aggroRange = 0.f;
	std::function<void()> m_attackInput;

	// the target to be destroyed!
	LevelMovableGameObject* m_currentTarget;
	std::vector<GameObject*>* m_enemies;
};