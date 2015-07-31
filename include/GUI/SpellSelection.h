#pragma once

#include "global.h"
#include "SpellManager.h"
#include "GUI/SpellSlot.h"

// the spell selection in a level
class SpellSelection
{
public:
	SpellSelection(SpellManager* manager);
	~SpellSelection();

	void show();
	void hide();

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

private:
	SpellManager* m_spellManager;
	const std::map<SpellID, sf::Time>& m_cooldownMap;
	std::map<SpellID, bool> m_activateMap;
	bool m_isVisible = true;

	std::map<SpellID, SpellSlot> m_spellSlots;

	// reloads the spells, depending on the manager
	void reload();

	// as seen from the bottom left corner
	const sf::Vector2f SPELLSELECTION_OFFSET = sf::Vector2f(10.f, 10.f);
	// x space between two slots
	const float SPELLSLOT_SPACING = 20.f;
};