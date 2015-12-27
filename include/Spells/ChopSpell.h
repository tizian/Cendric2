#pragma once

#include "global.h"
#include "Spell.h"

class ChopSpell : public Spell {
public:
	ChopSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;

private:
	sf::Vector2f m_spellOffset;
};