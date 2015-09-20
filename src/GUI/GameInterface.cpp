#include "GUI/GameInterface.h"
#include "Screens/GameScreen.h"

GameInterface::GameInterface(GameScreen* screen) 
{
	m_screen = screen;
	m_core = screen->getCharacterCore();
}

GameInterface::~GameInterface()
{
}

void GameInterface::render(sf::RenderTarget& target)
{
	target.setView(target.getDefaultView());

	m_characterInfo->render(target);
	m_spellbook->render(target);
	m_questLog->render(target);
	m_inventory->render(target);
}

void GameInterface::update(const sf::Time& frameTime)
{
	updateInventory(frameTime);
	updateSpellbook(frameTime);
	updateQuestLog(frameTime);
	updateCharacterInfo();
}

void GameInterface::reloadInventory(const std::string& changedItemID)
{
	m_inventory->notifyChange(changedItemID);
}

void GameInterface::reloadQuestLog()
{
	m_questLog->reload();
}

void GameInterface::updateCharacterInfo()
{
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo))
	{
		if (!m_characterInfo->isVisible())
		{
			if (m_inventory->isVisible()) m_inventory->hide();
			if (m_spellbook->isVisible()) m_spellbook->hide();
			if (m_questLog->isVisible()) m_questLog->hide();

			m_characterInfo->show();
		}
		else
		{
			m_characterInfo->hide();
		}
	}
	else if (m_characterInfo->isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_characterInfo->hide();
		g_inputController->lockAction();
	}
}

void GameInterface::updateSpellbook(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Spellbook))
	{
		if (!m_spellbook->isVisible())
		{
			if (m_characterInfo->isVisible()) m_characterInfo->hide();
			if (m_inventory->isVisible()) m_inventory->hide();
			if (m_questLog->isVisible()) m_questLog->hide();

			m_spellbook->show();
		}
		else
		{
			m_spellbook->hide();
		}
	}
	else if (m_spellbook->isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_spellbook->hide();
		g_inputController->lockAction();
	}

	m_spellbook->update(frameTime);
}

void GameInterface::updateInventory(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Inventory))
	{
		if (!m_inventory->isVisible())
		{
			if (m_characterInfo->isVisible()) m_characterInfo->hide();
			if (m_spellbook->isVisible()) m_spellbook->hide();
			if (m_questLog->isVisible()) m_questLog->hide();

			m_inventory->show();
		}
		else
		{
			m_inventory->hide();
		}
	}
	else if (m_inventory->isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_inventory->hide();
		g_inputController->lockAction();
	}

	m_inventory->update(frameTime);
}

void GameInterface::updateQuestLog(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Journal))
	{
		if (!m_questLog->isVisible())
		{
			if (m_characterInfo->isVisible()) m_characterInfo->hide();
			if (m_spellbook->isVisible()) m_spellbook->hide();
			if (m_inventory->isVisible()) m_inventory->hide();

			m_questLog->show();
		}
		else
		{
			m_questLog->hide();
		}
	}
	else if (m_questLog->isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_questLog->hide();
		g_inputController->lockAction();
	}

	m_questLog->update(frameTime);
}

CharacterCore* GameInterface::getCore() const
{
	return m_core;
}

Inventory* GameInterface::getInventory() const
{
	return m_inventory;
}