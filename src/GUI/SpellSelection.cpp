#include "GUI/SpellSelection.h"

SpellSelection::SpellSelection(SpellManager* manager) : m_cooldownMap(manager->getCooldownMap())
{
	m_spellManager = manager;
	m_activateMap.clear();
	for (auto& it : m_cooldownMap)
	{
		m_activateMap.insert({it.first, false});
	}
	reload();
}

SpellSelection::~SpellSelection()
{
	m_activateMap.clear();
	m_spellSlots.clear();
}

void SpellSelection::update(const sf::Time& frametime)
{
	for (auto& it : m_cooldownMap)
	{
		if (!m_activateMap[it.first] && it.second > sf::Time::Zero)
		{
			m_activateMap[it.first] = true;
			m_spellSlots[it.first].activate(it.second);
			m_spellSlots[it.first].playAnimation();
		} 
		else if (m_activateMap[it.first] && it.second == sf::Time::Zero)
		{
			m_activateMap[it.first] = false;
		}
	}

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
			it.second->getSpellBean().color);
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