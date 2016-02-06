#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"
#include "Structs/MovingTileData.h"
#include "GUI/BitmapText.h"

class MovingTile : public virtual LevelDynamicTile, public virtual MovableGameObject {
public:
	MovingTile(Level* level) : LevelDynamicTile(level) {} 
	~MovingTile() { delete m_debugInfo; }
	void setMovingTileData(const MovingTileData& data);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void setPosition(const sf::Vector2f& position) override;

	void renderAfterForeground(sf::RenderTarget& target) override;
	void setDebugBoundingBox(const sf::Color &debugColor) override;

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

	// debug info
	BitmapText* m_debugInfo = nullptr;
};