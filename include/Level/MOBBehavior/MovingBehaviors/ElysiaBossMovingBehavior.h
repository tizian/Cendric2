#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

class ElysiaBoss;

// A moving behavior for the mob "ElysiaBoss"
class ElysiaBossMovingBehavior : public virtual FlyingBehavior {
public:
	ElysiaBossMovingBehavior(ElysiaBoss* enemy);

	void update(const sf::Time& frameTime) override;
	void execHandleMovementInput() override;

private:
	ElysiaBoss* m_boss;

	void handleProjectileMovement();
	void handleNosediveDown();
	void handleNosediveUp();
	void handleThunderUp();
	void handleFlyToTarget(float appDist, const sf::Vector2f& target);

	sf::Vector2f m_noseTargetLeft;
	sf::Vector2f m_noseTargetRight;
	sf::Vector2f m_thunderTarget;
	sf::Vector2f m_flyingTarget;
	bool m_isNoseRight;

	// boss state transitions
	void updateBossState(const sf::Time& frameTime);
	sf::Time m_timeUntilTransition;

	static const sf::Time PROJECTILE_TIME;
};