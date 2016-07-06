#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellData.h"

class ShootingTile : public virtual LevelDynamicTile {
public:
	ShootingTile(LevelScreen* levelScreen);

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Shooting; }

private:
	std::string getSpritePath() const override;
	void loadSpells();
	void executeSpells();

private:
	int m_skinNr = 0;
	bool m_isBroken = false;
	// animation time of "Active"
	sf::Time m_activeTime = sf::Time::Zero;
	sf::Time m_remainingActiveTime = sf::Time::Zero;
	// recovering time: if the tile's state is broken or dead, it will recover after this time
	sf::Time m_recoveringTime = sf::Time::Zero;
	sf::Time m_remainingRecoveringTime = sf::Time::Zero;
	// spell offset time
	sf::Time m_spellOffsetTime = sf::Time::Zero;
	sf::Time m_remainingSpellOffsetTime = sf::Time::Zero;
	// cooldown for the next spell
	sf::Time m_cooldown = sf::Time::Zero;
	sf::Time m_remainingCooldown = sf::Time::Zero;
	SpellData m_spellData;
};