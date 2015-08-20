#pragma once

#include "global.h"
#include "Spell.h"
#include "Particles/ParticleSystem.h"

class WindGustSpell : public Spell
{
public:
	WindGustSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredIsAttachedToMob() const override;
	bool getConfiguredRotateSprite() const override;

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::PointPositionGenerator* m_pointGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};