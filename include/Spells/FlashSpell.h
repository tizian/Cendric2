#pragma once

#include "global.h"
#include "Spell.h"

class FlashSpell : public Spell {
public:
	FlashSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void execOnHit(LevelMovableGameObject* target) override;

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_posGenerator;

	bool m_isFlashingRight;

	void loadParticleSystem();

	void updateParticleSystemPosition();

	LevelMovableGameObject* m_mob;
};