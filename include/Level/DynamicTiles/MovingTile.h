#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"
#include "Structs/MovingTileData.h"

class MovingTile : public LevelDynamicTile, public MovableGameObject {
public:
	MovingTile(Level* level) : LevelDynamicTile(level) {}
	void setMovingTileData(const MovingTileData& data);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void setPosition(const sf::Vector2f& position) override;

	GameObjectType getConfiguredType() const override;
	 
private:
	void setFrozen(bool frozen);
	bool m_isOnHold;
	bool m_isFrozen;
	sf:: Vector2f m_currentVelocity;
	sf::Time m_distanceTime;
	sf::Time m_timeUntilTurn;

	std::vector<sf::Sprite> m_normalSprites;
	std::vector<sf::Sprite> m_frozenSprites;
};