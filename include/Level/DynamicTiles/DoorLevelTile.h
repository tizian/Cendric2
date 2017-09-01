#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "World/DoorTile.h"

class DoorLevelTile final : public virtual LevelDynamicTile, public virtual DoorTile {
public:
	DoorLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void onRightClick() override;

	// resolving diamond of death
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Door; }
	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }
	void render(sf::RenderTarget& target) override { LevelDynamicTile::render(target); }

private:
	std::string getSpritePath() const override;
	void open() override;
	void close() override;
	float getOpenRange() const override;

private:
	bool m_isInitiallyCollidable;
	int m_tileWidth;
};
