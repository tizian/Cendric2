#pragma once

#include "global.h"
#include "Spell.h"
#include "Particles/ParticleSystem.h"

class WindGustSpell : public Spell {
public:
	WindGustSpell(int strength);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;
	float getPushAcceleration() const;

private:
	float m_pushAcceleration;
	int m_strength;
	bool m_hasDamaged = false; // this spell only applies its dot once.
	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_pointGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};