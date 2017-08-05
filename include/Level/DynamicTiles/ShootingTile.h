#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Level/DynamicTiles/LeverDependentTile.h"
#include "Structs/SpellData.h"

class ShootingTile final : public LevelDynamicTile, public LeverDependentTile {
public:
	ShootingTile(LevelScreen* levelScreen);

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Shooting; }

	void switchTile() override;
	bool isSwitchable() const override;

private:
	std::string getSpritePath() const override;
	void loadSpells();
	void executeSpells();
	void setInitialState(bool on) override;

private:
	int m_skinNr = 0;
	bool m_isBroken = false;
	bool m_isInvincible = false;
	bool m_isInactive = false;
	bool m_isOnce = false;
	// animation time of "Active"
	sf::Time m_activeTime = sf::Time::Zero;
	sf::Time m_remainingActiveTime = sf::Time::Zero;
	// recovering time: if the tile's state is broken it will recover after this time
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