#pragma once

#include "global.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"

// An allied enemy attacking behavior. These enemies help cendric and attack unfriendly enemies
class AllyBehavior final : public EnemyAttackingBehavior {
public:
	AllyBehavior(Enemy* enemy);
	~AllyBehavior() {};

	void update(const sf::Time& frameTime) override;
	void updateAggro() override;

	// sets the time to live for an allied enemy
	void setTimeToLive(const sf::Time& ttl);

	sf::Color getConfiguredHealthColor() const override;

private:
	sf::Time m_timeToLive = sf::Time::Zero;
	bool m_hasTimeToLive = false;
};