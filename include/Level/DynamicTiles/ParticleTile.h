#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "LightObject.h"
#include "Particles/ParticleSystem.h"

// available skins:
// 1: fire
// 2: blue fire
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
	particles::TextureParticleSystem* m_ps = nullptr;
	particles::AimedVelocityGenerator* m_velGenerator = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;

	void loadParticleSystem(int skinNr);
};