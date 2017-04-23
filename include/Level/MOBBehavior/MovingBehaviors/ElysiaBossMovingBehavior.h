#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

class ElysiaBoss;

// A moving behavior for the mob "ElysiaBoss"
class ElysiaBossMovingBehavior final : public FlyingBehavior {
public:
	ElysiaBossMovingBehavior(ElysiaBoss* enemy);
	~ElysiaBossMovingBehavior();

	void update(const sf::Time& frameTime) override;
	void execHandleMovementInput() override;
	void updateAnimation(const sf::Time& frameTime) override;

private:
	ElysiaBoss* m_boss;

	void handleProjectileMovement();
	void handleNosediveDown();
	void handleNosediveUp();
	void handlePreThunder();
	void handleThunderUp();
	void handleThunderstorm();
	void handleFlyToTarget(float appDist, const sf::Vector2f& target);
	void initTiles();

	sf::Vector2f m_noseTargetLeft;
	sf::Vector2f m_noseTargetRight;
	sf::Vector2f m_thunderTop;
	sf::Vector2f m_thunderBottom;
	sf::Vector2f m_flyingTarget;
	bool m_isNoseRight;
	float m_nosediveTrajHeight;

	sf::Time m_timeUntilTransition;
	sf::Time m_frameTime;
	sf::Time m_nosediveTime;
	float m_nosediveStartX;
	float m_nosediveEndX;
	sf::Vector2f m_bossBoxOffset;

	static const sf::Time PROJECTILE_TIME;
	static const sf::Time NOSEDIVE_TIME;
	static const sf::Time THUNDER_TIME;
	
	sf::Time m_timeUntilNextThunder;
	sf::Time m_nextThunderTime;
	float m_thunderStartY;

	int m_thunderProbability = 20;

	// holder for her tiles
	std::vector<std::pair<GameObject*, sf::Vector2f>> m_cloudTiles;
	std::vector<std::pair<GameObject*, float>> m_damagingTiles;
};