#pragma once

#include "global.h"
#include "Spells/Spell.h"

/*
projectiles that rotate
*/
class RotatingProjectileSpell final : public Spell {
public:
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) override;

	void update(const sf::Time& frameTime) override;

	bool getConfiguredRotateSprite() const override;

private:
	void init(const SpellData& data);

private:
	float m_angle;
};