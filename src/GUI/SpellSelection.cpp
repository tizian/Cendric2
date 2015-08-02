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

void SpellSelection::activateSlot(SpellID spell, const sf::Time& cooldown)
{
	m_spellSlots[spell].playAnimation(cooldown);
}

void SpellSelection::update(const sf::Time& frametime)
{
	for (auto& it : m_spellSlots)
	{
		it.second.update(frametime);
	}
}

void SpellSelection::render(sf::RenderTarget& target) 
{
	if (m_isVisible)
	{
		for (auto& it : m_spellSlots)
		{
			it.second.render(target);
		}
	}
}

void SpellSelection::reload()
{
	m_spellSlots.clear();
	float offset = SpellSlot::RADIUS;
	for (auto& it : m_spellManager->getSpellMap())
	{
		SpellSlot slot(
			sf::Vector2f(SPELLSELECTION_OFFSET.x + offset, WINDOW_HEIGHT - (SpellSlot::RADIUS + SPELLSELECTION_OFFSET.y)),
			it.second->getSpellBean());
		m_spellSlots.insert({ it.first, slot });
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