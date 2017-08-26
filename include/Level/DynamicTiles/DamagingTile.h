#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class DamagingTile final : public LevelDynamicTile {
public:
	DamagingTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void update(const sf::Time& frameTime) override;
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override {};
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Damaging; }

private:
	std::string getSpritePath() const override;
	bool m_isDivine = false;
	bool m_isBroken = false;
	sf::Time m_despawnTime;

	static const std::string SHATTER_SOUND;
};