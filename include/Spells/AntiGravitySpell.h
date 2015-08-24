#pragma once

#include "global.h"
#include "Spell.h"

// not a spell projectile but it controls its mob. it is invisible for now (no texture), but that could change.
class AntiGravitySpell : public Spell
{
public:
	AntiGravitySpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;

private:
	AttributeBean m_additionalResistance;
};