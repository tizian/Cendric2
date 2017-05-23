#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "World/MovableGameObject.h"
#include "Structs/JumpingTileData.h"
#include "Structs/DamageOverTimeData.h"

// skins:
// 0: piranha
// 1: fireball
// 2: shadow piranha
// 3: toxic ball
class JumpingTile final : public LevelDynamicTile, public MovableGameObject {
public:
	JumpingTile(LevelScreen* levelScreen);

	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }
	void render(sf::RenderTarget& target) override { LevelDynamicTile::render(target); }
	void renderAfterForeground(sf::RenderTarget& target) override { MovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& debugColor) override { MovableGameObject::setDebugBoundingBox(debugColor); }

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void onHit(LevelMovableGameObject* mob) override;
	void setJumpingTileData(const JumpingTileData& data);

	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Jumping; }

private:
	void checkCollisions(const sf::Vector2f& nextPosition);
	void changeDirection();
	std::string getSpritePath() const override;

private:
	const float GRAVITY_ACCELERATION = 1000.f;
	const float AGGRO_DISTANCE = 200.f;
	
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