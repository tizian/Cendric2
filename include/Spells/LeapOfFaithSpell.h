#pragma once

#include "global.h"
#include "Spells/Spell.h"

class LeapOfFaithSpell : public Spell {
public:
	LeapOfFaithSpell(float gravityScale);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setDisposed() override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	float m_gravityScale;
	bool m_isFacingRight;

	std::unique_ptr<particles::TextureParticleSystem> m_ps = nullptr;
	particles::AngledVelocityGenerator* m_velGenerator = nullptr;
	particles::BoxPositionGenerator* m_pointGenerator = nullptr;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};