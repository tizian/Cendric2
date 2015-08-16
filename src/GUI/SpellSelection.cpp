#include "GUI/SpellSelection.h"

SpellSelection::SpellSelection(SpellManager* manager) 
{
	m_spellManager = manager;
	m_spellManager->setSpellSelection(this);
	reload();
}

SpellSelection::~SpellSelection()
{
	m_spellSlots.clear();
}

void SpellSelection::activateSlot(int spellNr, const sf::Time& cooldown)
{
	m_spellSlots[spellNr].playAnimation(cooldown);
}

void SpellSelection::update(const sf::Time& frametime)
{
	for (auto& it : m_spellSlots)
	{
		it.update(frametime);
	}
}

void SpellSelection::render(sf::RenderTarget& target) 
{
	if (m_isVisible)
	{
		for (auto& it : m_spellSlots)
		{
			it.render(target);
		}
	}
}

void SpellSelection::reload()
{
	m_spellSlots.clear();
	float offset = SpellSlot::RADIUS;
	for (auto& it : m_spellManager->getSpellMap())
	{
		SpellSlot slot(it->getSpellBean());
		slot.setPosition(sf::Vector2f(
			SPELLSELECTION_OFFSET.x + offset, 
			WINDOW_HEIGHT - (SpellSlot::RADIUS + SPELLSELECTION_OFFSET.y)));
		m_spellSlots.push_back(slot);
		offset += (SPELLSLOT_SPACING + 2 * SpellSlot::RADIUS);
	}
}

void SpellSelection::show()
{
	m_isVisible = true;
}

void SpellSelection::hide()
{
	m_isVisible = false;
}