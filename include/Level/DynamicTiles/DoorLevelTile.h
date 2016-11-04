#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/DoorData.h"

class InteractComponent;

class DoorLevelTile : public LevelDynamicTile {
public:
	DoorLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void onRightClick() override;
	void setDoorData(const DoorData& data);
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Door; }

private:
	std::string getSpritePath() const override;
	void open();
	void close();

private:
	DoorData m_doorData;
	InteractComponent* m_interactComponent;
	bool m_isOpen;
	bool m_isInitialized = false;

	static const float OPEN_RANGE;
};
