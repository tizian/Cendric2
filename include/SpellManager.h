#pragma once

#include <map>

#include "global.h"
#include "Spell.h"
#include "InputController.h"
#include "Spells/FireSpell.h"
#include "Spells/ChopSpell.h"
#include "Spells/IceSpell.h"
#include "Spells/ForcefieldSpell.h"
#include "Spells/UnlockSpell.h"

// a class that decides whether a spell can be cast or not and adds modifiers. It also draws a part of the gui.
class SpellManager
{
public:
	SpellManager(const AttributeBean* bean);
	~SpellManager();

	void update(sf::Time frameTime);
	void render(sf::RenderTarget &renderTarget);
	void addSpell(SpellBean& spell);
	void setCurrentSpell(SpellID id);
	// returns a spell vector that can be added to the game screen. If the spell cannot be cast because of cd, returns an empty vector
	const std::vector<Spell*>& getSpells();
	
private:
	// updates the spells damage, using the mobs attributes. It adds damage and calculates critical hits
	void updateDamage(DamageType type, SpellBean& bean) const;
	SpellID m_currentSpell;
	std::map<SpellID, sf::Time> m_coolDownMap;
	std::map<SpellID, SpellBean> m_spellMap;
	std::vector<Spell*> m_spellHolder;
	const AttributeBean* m_attributeBean;
};