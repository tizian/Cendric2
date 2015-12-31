#pragma once

#include "global.h"
#include "Spell.h"

class InvisibilitySpell : public Spell {
public:
	InvisibilitySpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;

private:
	sf::Time m_smokeDuration;
	static const sf::Time SMOKE_DURATION;

	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::DiskPositionGenerator* m_posGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};