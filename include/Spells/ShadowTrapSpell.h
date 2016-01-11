#pragma once

#include "global.h"
#include "Spell.h"

class ShadowTrapSpell : public Spell {
public:
	ShadowTrapSpell(int strength);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void execOnHit(LevelMovableGameObject* target) override;

	sf::Vector2f getConfiguredPositionOffset() const override;

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	bool getConfiguredRotateSprite() const override;
	float getConfiguredMaxVelocityYDown() const override;

private:
	int m_strength;
	sf::Time m_stunDuration;
};