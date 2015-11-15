#pragma once

#include "global.h"
#include "Spell.h"

class TelekinesisSpell : public Spell {
public:
	TelekinesisSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_pointGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};