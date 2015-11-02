#pragma once

#include "global.h"
#include "Spell.h"

class LightSpell : public Spell {
public:
	LightSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setScreen(Screen* screen) override;
	void setDisposed() override;

	const sf::Vector2f getConfiguredPositionOffset() const override;

private:
	LightObject* m_lightObject = nullptr;

	std::unique_ptr<particles::TextureParticleSystem> m_ps = nullptr;
	particles::AngledVelocityGenerator* m_velGenerator = nullptr;
	particles::BoxPositionGenerator* m_pointGenerator = nullptr;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};