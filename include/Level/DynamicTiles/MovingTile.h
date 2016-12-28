#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"
#include "Level/DynamicTiles/LeverDependentTile.h"
#include "Structs/MovingTileData.h"

class MovingTile : public virtual LevelMovableTile, public virtual LeverDependentTile {
public:
	MovingTile(LevelScreen* levelScreen);
	void setMovingTileData(const MovingTileData& data);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void setPosition(const sf::Vector2f& position) override;

	const sf::Vector2f& getRelativeVelocity() const;

	void setInitialState(bool on) override;
	void switchTile() override;
	bool isSwitchable() const override;

	// those methods are overridden to resolve the MI diamond of death:
	void updateFirst(const sf::Time& frameTime) override { LevelMovableTile::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { LevelMovableTile::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& debugColor) override { LevelMovableTile::setDebugBoundingBox(debugColor); }
	GameObjectType getConfiguredType() const override { return LevelMovableTile::getConfiguredType(); }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Moving; } 

protected:
	bool collides(const sf::Vector2f& nextPos) const override { return false; }

private:
	void updateRelativeVelocity(const sf::Time& frameTime) override;
	std::string getSpritePath() const override;
	void setFrozen(bool frozen, bool permanent = false);
	bool m_isFrozen;
	bool m_isPermanentlyFrozen;
	bool m_isActive;
	sf::Vector2f m_currentVelocity;
	sf::Time m_distanceTime;
	sf::Time m_timeUntilTurn;
	sf::Time m_frozenTime;

	static const sf::Time FROZEN_TIME;
	static const sf::Time FROZEN_FADING_TIME;

	std::vector<sf::Sprite> m_normalSprites;
	std::vector<sf::Sprite> m_frozenSprites;

	sf::Vector2f m_relativeVelocity;
};