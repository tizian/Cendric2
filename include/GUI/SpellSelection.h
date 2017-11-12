#pragma once

#include "global.h"
#include "Spells/SpellManager.h"
#include "GUI/SpellSlot.h"

// the spell selection in a level
class SpellSelection final {
public:
	SpellSelection(SpellManager* manager);
	~SpellSelection();

	void show();
	void hide();
	// called by the spell manager 
	void activateSlot(int spellNr, const sf::Time& cooldown);
	// called by the spell manager
	void selectSlot(int spellNr);

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);
	void reload();

	bool isSlotLocked(int spellNr) const;
	std::vector<SpellSlot>& getSlots();

private:
	SpellManager* m_spellManager;
	bool m_isVisible = true;

	std::vector<SpellSlot> m_spellSlots;

	// as seen from the bottom left corner
	const sf::Vector2f SPELLSELECTION_OFFSET = sf::Vector2f(10.f, 20.f);
	// x space between two slots
	const float SPELLSLOT_SPACING = 5.f;
	int m_selectedSlot = 0;
};