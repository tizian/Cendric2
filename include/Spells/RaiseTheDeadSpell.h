#pragma once

#include "global.h"
#include "Spells/Spell.h"

class RaiseTheDeadSpell final : public Spell {
public:
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

protected:
	bool checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) override;
	void loadComponents();
};