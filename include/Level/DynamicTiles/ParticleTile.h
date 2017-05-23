#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "World/LightObject.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;

// available skins:
// 1: fire
// 2: blue fire
// 3: toxic water
// 4: green fire
// 5: green embers
// 6: purple embers
class ParticleTile final : public LevelDynamicTile {
public:
	ParticleTile(LevelScreen* levelScreen);

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
	ParticleComponent* m_pc;
	particles::AimedVelocityGenerator* m_velGen = nullptr;
	sf::RenderTexture* getParticleTexture();

	void loadFlameParticles(int skinNr);
	void loadWaterParticles(int skinNr);
	void loadEmberParticles(int skinNr);
	void loadParticleSystem(int skinNr);
};