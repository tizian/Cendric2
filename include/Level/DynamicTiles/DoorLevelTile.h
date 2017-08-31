#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class InteractComponent;

class DoorLevelTile final : public LevelDynamicTile {
public:
	DoorLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void onRightClick() override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Door; }

private:
	std::string getSpritePath() const override;
	void open();
	void close();

private:
	InteractComponent* m_interactComponent;
	bool m_isOpen;
	bool m_isInitialized = false;
	int m_tileWidth = 1;
	int m_strength = 0;
	std::string m_keyItemID;
	std::vector<Condition> m_conditions;
	bool m_isInitiallyCollidable;

	static const float OPEN_RANGE;
};
