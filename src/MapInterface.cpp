#include "MapInterface.h"

MapInterface::MapInterface(CharacterCore* core) :
m_core(core),
m_inventory(this),
m_characterInfo(&core->getTotalAttributes()),
m_spellbook(core, true)
{
}

MapInterface::~MapInterface()
{
}

void MapInterface::render(sf::RenderTarget& target)
{
	target.setView(target.getDefaultView());
	m_characterInfo.render(target);
	m_inventory.render(target);
	m_spellbook.render(target);
}

void MapInterface::update(const sf::Time& frameTime)
{
	updateInventory(frameTime);
	updateCharacterInfo();
	updateSpellbook(frameTime);
}

void MapInterface::reloadInventory()
{
	if (m_inventory.isVisible())
	{
		m_inventory.reload();
	}
}

void MapInterface::updateCharacterInfo()
{
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo))
	{
		if (!m_characterInfo.isVisible())
		{
			if (m_inventory.isVisible())
			{
				m_inventory.hide();
			}
			if (m_spellbook.isVisible())
			{
				m_spellbook.hide();
			}
			m_characterInfo.show();
		}
		else
		{
			m_characterInfo.hide();
		}
	}
	else if (m_characterInfo.isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_characterInfo.hide();
		g_inputController->lockAction();
	}
}

void MapInterface::updateInventory(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Inventory))
	{
		if (!m_inventory.isVisible())
		{
			if (m_characterInfo.isVisible())
			{
				m_characterInfo.hide();
			}
			if (m_spellbook.isVisible())
			{
				m_spellbook.hide();
			}
			m_inventory.show();
			m_inventory.reload();
		}
		else
		{
			m_inventory.hide();
		}
	}
	else if (m_inventory.isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_inventory.hide();
		g_inputController->lockAction();
	}

	m_inventory.update(frameTime);
}

void MapInterface::updateSpellbook(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Spellbook))
	{
		if (!m_spellbook.isVisible())
		{
			if (m_characterInfo.isVisible())
			{
				m_characterInfo.hide();
			}
			if (m_inventory.isVisible())
			{
				m_inventory.hide();
			}
			m_spellbook.reload();
			m_spellbook.show();
		}
		else
		{
			m_spellbook.hide();
		}
	}
	else if (m_spellbook.isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_spellbook.hide();
		g_inputController->lockAction();
	}

	m_spellbook.update(frameTime);
}

CharacterCore* MapInterface::getCore() const
{
	return m_core;
}
