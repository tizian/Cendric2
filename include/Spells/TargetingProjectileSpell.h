#pragma once

#include "global.h"
#include "Spells/Spell.h"

/*
projectiles that have a target (the main char) and adjust their trajectory a bit
*/
class TargetingProjectileSpell : public Spell {
public:
	virtual ~TargetingProjectileSpell();

	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	float getConfiguredMaxVelocityX() const override { return m_data.speed; }
	float getConfiguredMaxVelocityYDown() const override { return m_data.speed; }
	float getConfiguredMaxVelocityYUp() const override { return m_data.speed; }

private:
	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();
	void updateParticleSystemPosition();

	void init(const SpellData& data);
};
