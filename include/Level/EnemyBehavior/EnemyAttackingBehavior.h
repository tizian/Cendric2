#pragma once

#include "global.h"
#include "Level/EnemyBehavior/AttackingBehavior.h"
#include "Enums/EnemyAttitude.h"

// An attacking behavior for enemies
class EnemyAttackingBehavior : virtual public AttackingBehavior {
public:
	EnemyAttackingBehavior(Enemy* enemy);
	virtual ~EnemyAttackingBehavior() {};

	virtual void update(const sf::Time& frameTime) override;

	virtual void updateAggro() = 0;

	void setAggroRange(float range);

	const LevelMovableGameObject* getCurrentTarget() const;
	float getAggroRange() const;
	float distToTarget() const;

	virtual EnemyAttitude getAttitude() const = 0;
	virtual sf::Color getConfiguredHealthColor() const = 0;

protected:
	Enemy* m_enemy;

	float m_aggroRange = 0.f;
	std::function<void()> m_attackInput;

	// the target to be destroyed!
	LevelMovableGameObject* m_currentTarget;
	std::vector<GameObject*>* m_enemies;
};