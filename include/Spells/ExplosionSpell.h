#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

class ParticleComponent;
class LightComponent;

// this spell is used by the boss: Yasha and Velius
// Yasha uses it with strength 1 while Velius has strength 0
// Strength > 0 will kill the main char on collision

// skins:
// 0: Yasha's explosion
// 1: Twilight explosion (velius)
// 2: Necromancy explosion (velius)
// 3: Divine explosion (velius)
class ExplosionSpell final : public Spell {
public:
	ExplosionSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;

private:
	void loadComponents();
	ParticleComponent* m_pc;
	LightComponent* m_lc;

	sf::Time m_particleTime;
	sf::Time m_lightTime;
	int m_iteration;
};