#pragma once

#include "global.h"
#include "Spells/Spell.h"

class RaiseTheDeadSpell : virtual public Spell {
public:
	virtual ~RaiseTheDeadSpell() {};
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

protected:
	bool checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) override;
};