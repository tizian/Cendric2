#pragma once

#include "global.h"
#include "Spells/Spell.h"

class IcyAmbushSpell : public Spell {
public:
	IcyAmbushSpell(int strength);
	virtual ~IcyAmbushSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	int m_strength;

	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};