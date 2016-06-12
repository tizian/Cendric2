#pragma once

#include "global.h"
#include "Spells/Spell.h"

class InvisibilitySpell : public Spell {
public:
	InvisibilitySpell();
	virtual ~InvisibilitySpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	void setDisposed() override;

	bool getConfiguredRotateSprite() const override;

private:
	sf::Time m_smokeDuration;
	static const sf::Time SMOKE_DURATION;

	particles::TextureParticleSystem* m_ps;
	particles::ParticleSpawner* m_particleSpawner;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};