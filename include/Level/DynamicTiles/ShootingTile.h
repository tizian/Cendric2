#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"
#include "Structs/DamageOverTimeData.h"

class ShootingTile : public virtual LevelDynamicTile {
public:
	ShootingTile(LevelScreen* levelScreen);

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;

private:
	
	sf::Vector2f m_initialVelocity;
	sf::Time m_waitingSpan = sf::Time::Zero;
	sf::Time m_waitingTime = sf::Time::Zero;
	sf::Time m_damageCooldown = sf::Time::Zero;
	bool m_isWaiting = false;
	bool m_isAggro = false;
	bool m_isAlternating = false;
	bool m_isReturning = false;
	bool m_isMelting = false;
	DamageOverTimeData m_damage;
	sf::Vector2f m_initialPosition;
};