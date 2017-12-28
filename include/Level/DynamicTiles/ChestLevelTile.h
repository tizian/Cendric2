#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "GUI/LootWindow.h"
#include "LuaBridge/LuaBridge.h"
#include "World/ChestTile.h"

class LevelMainCharacter;
class InteractComponent;
class LightComponent;
class WorldCallback;

class ChestLevelTile final : public LevelDynamicTile, public ChestTile {
public:
	ChestLevelTile(LevelScreen* levelScreen);
	~ChestLevelTile();
	void renderAfterForeground(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;
	void onMouseOver() override;
	void loot();
	
	bool isLootable() const;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Chest; }

private:
	std::string getSpritePath() const override;
	void setLoot(const std::map<std::string, int>& items, int gold);
	void unlock(bool soundOn);
	bool m_isLootable;

	InteractComponent* m_interactComponent = nullptr;
	// The light will disappear (set invisible) on looted.
	LightComponent* m_lightComponent = nullptr;

	// the chest can only be looted if the main char is in this range
	static const float PICKUP_RANGE;
	LootWindow* m_lootWindow = nullptr;
	bool m_showLootWindow = false;

	// world callback
	WorldCallback* m_worldCallback = nullptr;
	luabridge::lua_State* m_L = nullptr;
	bool loadLua();
	void executeOnLoot() const;
};