#pragma once

#include "global.h"
#include "Spells/RaiseTheDeadSpell.h"

class YashaRaiseTheDeadSpell final : public RaiseTheDeadSpell {
public:
	void update(const sf::Time& frameTime) override;
	void execOnHit(LevelMovableGameObject* target) override;

protected:
	bool checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) override;
};