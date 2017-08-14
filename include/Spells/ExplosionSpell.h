#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;

// this spell is used by the boss: Yasha
class ExplosionSpell final : public Spell {
public:
	ExplosionSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;

private:
	void loadComponents();
	ParticleComponent* m_pc;

	sf::Time m_particleTime;
};