#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

namespace particles {
	class LineSpawner;
}

class AntiMagicTile final : public LevelDynamicTile {
public:
	AntiMagicTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void setPosition(const sf::Vector2f& pos) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override {};
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::AntiMagic; }

private:
	std::string getSpritePath() const override;
	void loadParticleSystem();

	float m_width;

	sf::Vector2f m_startPos;
	sf::Vector2f m_endPos;

	sf::Sprite m_otherSprite;
	particles::LineSpawner* m_lineSpawner = nullptr;
};