#include "LevelInterface.h"
#include "LevelMainCharacter.h"

LevelInterface::LevelInterface(CharacterCore* core, LevelMainCharacter* character) : 
m_core(core), 
m_character(character),
m_inventory(this), 
m_characterInfo(character->getAttributes()), 
m_healthBar(character->getAttributes()), 
m_quickSlotBar(this),
m_spellbook(core, false),
m_questLog(core)
{
}

LevelInterface::~LevelInterface()
{
	delete m_spellSelection;
}

void LevelInterface::render(sf::RenderTarget& target)
{
	target.setView(target.getDefaultView());

	m_healthBar.render(target);
	m_buffBar.render(target);
	m_spellSelection->render(target);
	m_quickSlotBar.render(target);
	m_characterInfo.render(target);
	m_spellbook.render(target);
	m_questLog.render(target);
	m_inventory.render(target);
}

void LevelInterface::update(const sf::Time& frameTime)
{
	m_healthBar.update();
	m_buffBar.update(frameTime);
	m_spellSelection->update(frameTime);
	m_quickSlotBar.update(frameTime);
	updateInventory(frameTime);
	updateSpellbook(frameTime);
	updateQuestLog(frameTime);
	updateCharacterInfo();
}

void LevelInterface::addBuff(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration)
{
	m_buffBar.addSlot(type, textureLocation, duration);
}

void LevelInterface::notifyConsumableDrop(const InventorySlotClone* item)
{
	m_quickSlotBar.notifyConsumableDrop(item);
}

void LevelInterface::consumeItem(const Item& item)
{
	m_character->consumeFood(
		item.getBean().foodDuration,
		item.getAttributes());
	addBuff(BuffType::Food,
		sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, 50, 50),
		item.getBean().foodDuration);
	m_core->removeItem(item.getID(), 1);
	m_inventory.reload();
	m_quickSlotBar.reload();
}

void LevelInterface::highlightQuickslots(bool highlight)
{
	m_quickSlotBar.highlightSlots(highlight);
}

void LevelInterface::reloadInventory()
{
	if (m_inventory.isVisible())
	{
		m_inventory.reload();
	}
	m_quickSlotBar.reload();
}

void LevelInterface::updateCharacterInfo()
{
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo))
	{
		if (!m_characterInfo.isVisible())
		{
			if (m_inventory.isVisible()) m_inventory.hide();
			if (m_spellbook.isVisible()) m_spellbook.hide();
			if (m_questLog.isVisible()) m_questLog.hide();

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

void LevelInterface::updateSpellbook(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Spellbook))
	{
		if (!m_spellbook.isVisible())
		{
			if (m_characterInfo.isVisible()) m_characterInfo.hide();
			if (m_inventory.isVisible()) m_inventory.hide();
			if (m_questLog.isVisible()) m_questLog.hide();

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

void LevelInterface::updateInventory(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Inventory))
	{
		if (!m_inventory.isVisible())
		{
			if (m_characterInfo.isVisible()) m_characterInfo.hide();
			if (m_spellbook.isVisible()) m_spellbook.hide();
			if (m_questLog.isVisible()) m_questLog.hide();

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

void LevelInterface::updateQuestLog(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Journal))
	{
		if (!m_questLog.isVisible())
		{
			if (m_characterInfo.isVisible()) m_characterInfo.hide();
			if (m_spellbook.isVisible()) m_spellbook.hide();
			if (m_inventory.isVisible()) m_inventory.hide();

			m_questLog.show();
			m_questLog.reload();
		}
		else
		{
			m_questLog.hide();
		}
	}
	else if (m_questLog.isVisible() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		m_questLog.hide();
		g_inputController->lockAction();
	}

	m_questLog.update(frameTime);
}

void LevelInterface::setSpellManager(SpellManager* spellManager)
{
	// use this spell manager for the interface bar.
	delete m_spellSelection;
	m_spellSelection = new SpellSelection(spellManager);
}

CharacterCore* LevelInterface::getCore() const
{
	return m_core;
}

LevelMainCharacter* LevelInterface::getMainCharacter() const
{
	return m_character;
}
