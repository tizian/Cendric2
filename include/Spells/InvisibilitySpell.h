#pragma once

#include "global.h"
#include "Spells/Spell.h"

class ParticleComponent;

class InvisibilitySpell : public Spell {
public:
	InvisibilitySpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	void setDisposed() override;

	bool getConfiguredRotateSprite() const override;

private:
	sf::Time m_smokeDuration;
	static const sf::Time SMOKE_DURATION;
	ParticleComponent* m_pc;
	void loadComponents();
};