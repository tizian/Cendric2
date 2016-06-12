#pragma once

#include "global.h"
#include "Spells/Spell.h"

#include <memory>

class LightSpell : public Spell {
public:
	LightSpell() {};
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};