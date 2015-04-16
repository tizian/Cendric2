#pragma once

#include <map>

#include "global.h"
#include "Spell.h"
#include "InputController.h"
#include "FireSpell.h"
#include "ChopSpell.h"
#include "IceSpell.h"
#include "ForcefieldSpell.h"

// a class that decides whether a spell can be cast or not and adds modifiers. It also draws a part of the gui.
class SpellManager
{
public:
	SpellManager();
	~SpellManager();

	void update(sf::Time frameTime);
	void render(sf::RenderTarget &renderTarget);
	// returns a spell that can be added to the game screen. If the spell cannot be cast because of cd, returns nullptr.
	Spell* getSpell();

private:
	SpellID m_currentSpell;
	std::map<SpellID, sf::Time> m_coolDownMap;
	std::map<Key, SpellID> m_keyMap;
	std::map<SpellID, SpellBean> m_spellMap;
	// fill maps
	void init();
};