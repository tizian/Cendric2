#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;

// this spell is used by the boss: WolfMonster
// the main char is only hit if he is facing the boss.
class TransformBeamSpell : public Spell {
public:
	TransformBeamSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;

private:
	bool m_hasStunned = false;
	sf::Time m_graceTime = sf::Time::Zero;

	void loadComponents();
	ParticleComponent* m_pc;
};