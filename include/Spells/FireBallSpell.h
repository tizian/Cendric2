#pragma once

#include "global.h"
#include "Spell.h"
#include "LightObject.h"

class FireBallSpell : public Spell, public LightObject
{
public:
	FireBallSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setViewable(bool value) override { Spell::setViewable(value); };

	GameObjectType getConfiguredType() const override;
};