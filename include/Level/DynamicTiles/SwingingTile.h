#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SwingingTile final : public LevelDynamicTile {
public:
	SwingingTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	~SwingingTile();
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void update(const sf::Time& frametime) override;
	void render(sf::RenderTarget& target) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ }
	void setPosition(const sf::Vector2f& position) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Swinging; }


private:
	std::string getSpritePath() const override;
	int m_size;
	sf::Texture* m_texture = nullptr;
	float m_currentRotation = 0.f;
};