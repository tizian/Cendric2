#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"
#include "Level/DynamicTiles/MovingParent.h"

class BoatTile final : public LevelMovableTile, public MovingParent {
public:
	BoatTile(LevelScreen* levelScreen);
	~BoatTile();

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& target) override;
	void setPosition(const sf::Vector2f& position) override;
	void setMovingParent(MovingParent* parent) override {};
	void destroy();

	// those methods are overridden to resolve the MI diamond of death:
	void render(sf::RenderTarget& target) override { LevelMovableTile::render(target); }
	void setState(GameObjectState state, bool updateAnimation) override { AnimatedGameObject::setState(state, updateAnimation); }
	void setState(GameObjectState state) override { AnimatedGameObject::setState(state); }
	void updateFirst(const sf::Time& frameTime) override { LevelMovableTile::updateFirst(frameTime); }
	void setDebugBoundingBox(const sf::Color& debugColor) override { LevelMovableTile::setDebugBoundingBox(debugColor); }
	GameObjectType getConfiguredType() const override { return LevelMovableTile::getConfiguredType(); }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Boat; }

protected:
	bool collides(const sf::Vector2f& nextPos) const override { return LevelMovableTile::collides(nextPos); }

private:
	void checkForWind();
	void checkForEnemies();
	void updateRelativeVelocity(const sf::Time& frameTime) override;
	std::string getSpritePath() const override;
	sf::Vector2f m_pushAcceleration;
	std::vector<GameObject*>* m_spellVec;
	std::vector<LevelDynamicTile*> m_dynamicTileVec;
	sf::FloatRect m_sailBoundingBox;
	sf::RectangleShape m_sailDebugBox;
	bool m_isInitialized;
	bool m_isFacingRight;
	bool m_newIsFacingRight;
	GameObjectState m_newState;

	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING = 0.5f;
};