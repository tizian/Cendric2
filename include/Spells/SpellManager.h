#pragma once

#include <map>

#include "global.h"
#include "Spells/Spell.h"
#include "InputController.h"
#include "Structs/SpellModifier.h"
#include "Spells/SpellCreator.h"

class SpellSelection;

// a class that decides whether a spell can be cast or not and holds the creators for the corresponding spells
class SpellManager {
public:
	SpellManager(LevelMovableGameObject* owner);
	~SpellManager();

	void update(sf::Time frameTime);
	void addSpell(const SpellData& spell);
	void addSpell(const SpellData& spell, const std::vector<SpellModifier>& modifiers);
	void setCurrentSpell(int spellNr);
	// sets the current spell to spellNr and, if it doesn't need a target, executes it.
	void setAndExecuteSpell(int spellNr);
	// triggers the spell creator & executes the current spell if it are not on cooldown
	void executeCurrentSpell(const sf::Vector2f& target);

	// used by the spell interface
	std::vector<SpellCreator*>& getSpellMap();

	// clears all spells from the maps
	void clearSpells();

	void setSpellSelection(SpellSelection* spellSelection);

	// sets all spells in the map allied / not allied
	void setSpellsAllied(bool value);

private:

	int m_currentSpell;
	std::vector<sf::Time> m_coolDownMap;
	std::vector<SpellCreator*> m_spellMap;
	LevelMovableGameObject* m_owner;

	// only used by the level main character to show the spells in its interface
	SpellSelection* m_spellSelection = nullptr;
};