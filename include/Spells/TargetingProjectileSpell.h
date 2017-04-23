#pragma once

#include "global.h"
#include "Spells/Spell.h"

/*
projectiles that have a target (the main char) and adjust their trajectory a bit
*/
class TargetingProjectileSpell final : public Spell {
public:
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	float getConfiguredMaxVelocityX() const override { return m_data.speed; }
	float getConfiguredMaxVelocityYDown() const override { return m_data.speed; }
	float getConfiguredMaxVelocityYUp() const override { return m_data.speed; }

private:
	void loadComponents();
	void init(const SpellData& data);
};
