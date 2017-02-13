#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "LightObject.h"
#include "Particles/ParticleSystem.h"

// available skins:
// 1: fire
// 2: blue fire
// 3: toxic water
// 4: green fire
// 5: green embers
// 6: purple embers
class ParticleTile : public LevelDynamicTile {
public:
	ParticleTile(LevelScreen* levelScreen);
	virtual ~ParticleTile();

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Particle; }

private:
	bool m_isForegroundTile = false;
	sf::Vector2f m_particlePosOffset;
	particles::TextureParticleSystem* m_ps = nullptr;
	particles::ParticleGenerator* m_velGenerator = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;

	void loadFlameParticles(int skinNr);
	void loadWaterParticles(int skinNr);
	void loadEmberParticles(int skinNr);
	void loadParticleSystem(int skinNr);
};