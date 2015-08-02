#pragma once

#include <map>

#include "global.h"
#include "Spell.h"
#include "InputController.h"
#include "Structs/SpellModifier.h"
#include "SpellCreator.h"
#include "SpellCreators/FireBallSpellCreator.h"
#include "SpellCreators/IceBallSpellCreator.h"
#include "SpellCreators/DivineShieldSpellCreator.h"
#include "SpellCreators/AureolaSpellCreator.h"
#include "SpellCreators/ChopSpellCreator.h"

class SpellSelection;

// a class that decides whether a spell can be cast or not and holds the creators for the corresponding spells
class SpellManager
{
public:
	SpellManager(LevelMovableGameObject* owner);
	~SpellManager();

	void update(sf::Time frameTime);
	void addSpell(const SpellBean& spell);
	void addSpell(const SpellBean& spell, const std::vector<SpellModifier>& modifiers);
	void setCurrentSpell(SpellID id);
	// triggers the spell creator & executes the current spell if it are not on cooldown
	void executeCurrentSpell(const sf::Vector2f& target);

	// used by the spell interface
	std::map<SpellID, SpellCreator*>& getSpellMap();
	
	void setSpellSelection(SpellSelection* spellSelection);
	
private:
	
	SpellCreator* getSpellCreator(const SpellBean& bean, const std::vector<SpellModifier>& modifiers);
	SpellID m_currentSpell;
	std::map<SpellID, sf::Time> m_coolDownMap;
	std::map<SpellID, SpellCreator*> m_spellMap;
	LevelMovableGameObject* m_owner;

	// only used by the level main character to show the spells in its interface
	SpellSelection* m_spellSelection = nullptr;

};