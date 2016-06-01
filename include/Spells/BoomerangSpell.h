#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "LightObject.h"

/*
this projectile will fly in an ellipse and return to its owner.
the range from the spell data is the max range
the absolute range of the projectile is min(dist to target, max range) = 2*a value of the ellipse
b is a/2
*/
class BoomerangSpell : public Spell {
public:
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	void onOwnerDisposed() override;
	void setDisposed() override;

private:
	void init(const SpellData& data);

	// how long does the spell not hurt after it has hit something?
	static const sf::Time DAMAGE_INTERVAL;
	sf::Time m_damageTimeout = sf::Time::Zero;
	// how fast does the sprite rotate?
	static const float ROTATION_PER_S;
	float m_currentRotation = 0.f;

	// ellipse params
	float m_a;
	float m_b;
	float m_theta;
	float m_t;
	float m_u; // circumference
	int m_direction;  // 1 or -1, the trajectory direction
	sf::Vector2f m_center;
};