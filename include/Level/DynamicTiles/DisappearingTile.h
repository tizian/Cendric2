#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Particles/ParticleSystem.h"

// skins
// 0: 0.25s
// 1: 0.5s
// 2: 1s
// 3: 2s

// a tile that disappears after a certain time as soon as cendric touches it.
class DisappearingTile : public virtual LevelDynamicTile {
public:
	DisappearingTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Disappearing; }

private:
	std::string getSpritePath() const override;
	void checkForMainCharacter();

	// when Cendric touches the tile, this time starts to run out
	// its starting time is defined per skin
	bool m_isTouched = false;
	sf::Time m_criticalTime;
	sf::Time m_startCriticalTime;
	void initForSkinNr(int skinNr);
	sf::FloatRect m_checkBoundingBox;

	// particle system
	void loadParticleSystem();
	particles::TextureParticleSystem* m_ps = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;
	particles::ColorGenerator* m_colorGenerator = nullptr;
};