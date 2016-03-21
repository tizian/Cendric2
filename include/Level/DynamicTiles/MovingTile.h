#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"
#include "Structs/MovingTileData.h"

class MovingTile : public virtual LevelMovableTile {
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

private:
	void setFrozen(bool frozen);
	bool m_isFrozen;
	sf::Vector2f m_currentVelocity;
	sf::Time m_distanceTime;
	sf::Time m_timeUntilTurn;

	std::vector<sf::Sprite> m_normalSprites;
	std::vector<sf::Sprite> m_frozenSprites;

	sf::Vector2f m_relativeVelocity;
};