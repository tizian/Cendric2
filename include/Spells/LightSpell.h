#pragma once

#include "global.h"
#include "Spells/Spell.h"

class LightSpell final : public Spell {
public:
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void execOnHit(LevelMovableGameObject* target) override;
	void setDisposed() override;

private:
	void loadComponents();
};