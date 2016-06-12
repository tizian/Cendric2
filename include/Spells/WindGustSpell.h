#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class WindGustSpell : public Spell {
public:
	WindGustSpell(int strength);
	virtual ~WindGustSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	bool getConfiguredRotateSprite() const override;
	float getPushAcceleration() const;

private:
	float m_pushAcceleration;
	int m_strength;
	bool m_hasDamaged = false; // this spell only applies its dot once.
	particles::TextureParticleSystem* m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};