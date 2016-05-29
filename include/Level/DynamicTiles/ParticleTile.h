#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "LightObject.h"
#include "Particles/ParticleSystem.h"

// available skins:
// 1: fire
class ParticleTile : public LevelDynamicTile {
public:
	ParticleTile(LevelScreen* levelScreen);

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	void setPosition(const sf::Vector2f& pos) override;

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps = nullptr;
	particles::AngledVelocityGenerator* m_velGenerator = nullptr;
	particles::DiskPositionGenerator* m_diskPosGenerator = nullptr;

	void loadParticleSystem(int skinNr);
};