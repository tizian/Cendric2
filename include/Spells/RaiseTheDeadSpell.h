#pragma once

#include "global.h"
#include "Spell.h"

class RaiseTheDeadSpell : public Spell {
public:
	RaiseTheDeadSpell(int strength);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	int m_strength;
	// a bool to check an unique rising per spell.
	bool m_hasRisen = false;

	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_pointGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};