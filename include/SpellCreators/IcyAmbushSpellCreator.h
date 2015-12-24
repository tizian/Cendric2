#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/IcyAmbushSpell.h"

// a class that creates icy ambush spells
class IcyAmbushSpellCreator : public SpellCreator {
public:
	IcyAmbushSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;

private:
	void addDurationModifier(int level) override;

	sf::Time m_stunTime = sf::seconds(1);
};