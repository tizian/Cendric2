#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"
#include "Structs/WorldCollisionQueryRecord.h"

class LevelMovableTile : public virtual LevelDynamicTile, public virtual MovableGameObject {
public:
	LevelMovableTile(Level* level);
	virtual ~LevelMovableTile() {};
	
	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }
	void update(const sf::Time& frameTime) override { LevelDynamicTile::update(frameTime); MovableGameObject::update(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { MovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color &debugColor) override { MovableGameObject::setDebugBoundingBox(debugColor); }

	GameObjectType getConfiguredType() const override;

protected:
	void updateRelativeVelocity(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition);
};