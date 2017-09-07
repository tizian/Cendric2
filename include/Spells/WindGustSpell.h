#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;

class WindGustSpell final : public Spell {
public:
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void execOnHit(LevelMovableGameObject* target) override;
	void setPosition(const sf::Vector2f& pos) override;

	bool getConfiguredRotateSprite() const override;
	float getPushAcceleration() const;

private:
	float m_pushAcceleration;
	float m_absPushAcceleration;
	DamageType m_damageType;
	sf::Time m_timeUntilDamage = sf::Time::Zero; // this spell only applies its dot once per second

	void loadParticleSystem();
	void updateParticleSystemPosition(); 
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxSpawner* m_particleSpawner;
	ParticleComponent* m_pc = nullptr;
};