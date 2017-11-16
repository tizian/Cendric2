#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "GUI/LootWindow.h"

class LevelMainCharacter;
class InteractComponent;

class ResourceTile final : public LevelDynamicTile {
public:
	ResourceTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void update(const sf::Time& frameTime) override;
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onHit(Spell* spell) override {};
	void loot();
	
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Resource; }

public:
	static const std::string PICK_SOUND_PATH;

protected:
	std::string getSoundPath() const override { return PICK_SOUND_PATH;  }

private:
	void initializeResource(int skinNr);
	std::string getSpritePath() const override;
	// the message that pops up if the main char has not the right tool
	std::string m_noToolMessage;
	// the tool that is used to loot this tile
	std::string m_toolItemID;
	// loot that can be gotten from that tile
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;
	sf::Time m_lootTime = sf::Time::Zero;

	InteractComponent* m_interactComponent = nullptr;
	
	// the resource can only be gotten if the main char is in this range
	const float PICKUP_RANGE = 100.f;
	sf::Time m_pickCooldown = sf::Time::Zero;
	int m_pickCount = 0;
	void updatePicking(const sf::Time& frameTime);
};