#pragma once

#include "global.h"
#include "Spells/Spell.h"

class LightSpell : public Spell {
public:
	LightSpell() {};
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void execOnHit(LevelMovableGameObject* target) override;

	sf::Vector2f getConfiguredPositionOffset() const override;

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps = nullptr;
	particles::AngledVelocityGenerator* m_velGenerator = nullptr;
	particles::BoxPositionGenerator* m_pointGenerator = nullptr;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};