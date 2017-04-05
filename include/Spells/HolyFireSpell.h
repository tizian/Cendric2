#pragma once

#include "global.h"
#include "Spells/Spell.h"

class ParticleComponent;

/*
the following skins are available:
0: holy fire (default)
1: shadow fire
*/
class HolyFireSpell : public Spell {
public:
	HolyFireSpell() {};
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	bool m_hasDamaged = false; // this spell only applies its dot once.
	sf::Time m_emitTime = sf::seconds(1.f);

	ParticleComponent* m_pc;
	void loadComponents();
};