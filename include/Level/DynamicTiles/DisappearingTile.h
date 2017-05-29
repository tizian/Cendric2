#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class ParticleComponent;

// skins
// 0: 0.25s
// 1: 0.5s
// 2: 1s
// 3: 1.5s
// 4: 2s

// a tile that disappears after a certain time as soon as cendric touches it.
// these tiles respawn.
class DisappearingTile final : public LevelDynamicTile {
public:
	DisappearingTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Disappearing; }

private:
	std::string getSpritePath() const override;
	void checkForMainCharacter();

	// when Cendric touches the tile, this time starts to run out
	// its starting time is defined per skin
	bool m_isTouched = false;
	sf::Time m_criticalTime;
	sf::Color m_skinColor;
	int m_skinNr;
	void initForSkinNr();
	sf::FloatRect m_checkBoundingBox;

	void loadComponents();
	ParticleComponent* m_pc;

	static const sf::Time RESPAWN_TIME;
	sf::Time m_respawnTime = sf::Time::Zero;
	void respawn();

	void touch();
};