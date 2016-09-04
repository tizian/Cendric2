#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

class WardenEnemy;

// A moving behavior for observing spell enemies (observers and warden).
class WardenBehavior : public virtual EnemyMovingBehavior {
public:
	WardenBehavior(WardenEnemy* enemy);
	~WardenBehavior() {};

	void execHandleMovementInput() override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation(const sf::Time& frameTime) override;
	void makeRandomDecision() override;
	void handleDefaultAcceleration() override;

private:
	WardenEnemy* m_warden;

	// returns a random idle time between blinking and looking around
	sf::Time getIdleTime() const;
	sf::Time m_remainingAnimationTime = sf::Time::Zero;
	bool m_isIdle = false;
};