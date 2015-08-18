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
	if (spellNr < 0 || spellNr > m_spellSlots.size() - 1) return;
	m_spellSlots[spellNr].playAnimation(cooldown);
}

void SpellSelection::selectSlot(int spellNr)
{
	if (spellNr < 0 || spellNr > m_spellSlots.size() - 1) return;
	m_spellSlots[m_selectedSlot].deselect();
	m_spellSlots[spellNr].select();
	m_selectedSlot = spellNr;
}

void SpellSelection::update(const sf::Time& frametime)
{
	for (int i = 0; i < m_spellSlots.size(); i++)
	{
		m_spellSlots[i].update(frametime);
		if (m_spellSlots[i].isClicked())
		{
			m_spellManager->setCurrentSpell(i);
		}
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