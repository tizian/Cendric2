#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "Particles/ParticleSystem.h"

// this spell is used by the boss: WolfMonster
// the main char is only hit if he is facing the boss.
class TransformBeamSpell : public Spell {
public:
	TransformBeamSpell();
	virtual ~TransformBeamSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	bool getConfiguredRotateSprite() const override;

private:
	bool m_hasStunned = false;
	sf::Time m_timeSinceStart = sf::Time::Zero;
	particles::TextureParticleSystem* m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();
};