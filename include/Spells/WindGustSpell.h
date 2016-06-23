#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class WindGustSpell : public Spell {
public:
	WindGustSpell() : Spell() {};
	virtual ~WindGustSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	bool getConfiguredRotateSprite() const override;
	float getPushAcceleration() const;

private:
	float m_pushAcceleration;
	DamageType m_damageType;
	sf::Time m_timeUntilDamage = sf::Time::Zero; // this spell only applies its dot once per second
	particles::TextureParticleSystem* m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};