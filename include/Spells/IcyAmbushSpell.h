#pragma once

#include "global.h"
#include "Spell.h"


class IcyAmbushSpell : public Spell
{
public:
	IcyAmbushSpell(const sf::Time& stunDuration);
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	// is set to true when the spell has found its target
	bool m_isStunning = false;
	sf::Time m_stunDuration;

	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_pointGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};