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
	float offset = 0.f;
	for (auto& it : m_spellManager->getSpellMap())
	{
		SpellSlot slot(
			sf::Vector2f(SPELLSELECTION_OFFSET.x + offset, WINDOW_HEIGHT - SPELLSELECTION_OFFSET.y - 40.f), 
			it.second->getSpellBean().color);
		m_spellSlots.insert({ it.first, slot });
		offset += (SPELLSLOT_SPACING + 40.f);
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