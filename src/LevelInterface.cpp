#include "LevelInterface.h"
#include "LevelMainCharacter.h"

LevelInterface::LevelInterface(CharacterCore* core, LevelMainCharacter* character) :
    m_inventory(core),  m_characterInfo(character), m_healthBar(character->getAttributes())
{
}

LevelInterface::~LevelInterface()
{
	delete m_spellSelection;
}

void LevelInterface::render(sf::RenderTarget& target)
{
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());

	m_healthBar.render(target);
	m_spellSelection->render(target);
	m_characterInfo.render(target);
	m_inventory.render(target);

	target.setView(oldView);
}

void LevelInterface::update(const sf::Time& frameTime)
{
	m_healthBar.update();
	m_spellSelection->update(frameTime);
	updateInventory();
	updateCharacterInfo();
}

void LevelInterface::updateCharacterInfo()
{
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo))
	{
		m_showCharacterInfo = !m_showCharacterInfo;
		if (m_showCharacterInfo)
		{
			if (m_showInventory)
			{
				m_showInventory = false;
				m_inventory.hide();
			}
			m_characterInfo.show();
		}
		else
		{
			m_characterInfo.hide();
		}
	}
}

void LevelInterface::updateInventory()
{
	if (g_inputController->isKeyJustPressed(Key::Inventory))
	{
		m_showInventory = !m_showInventory;
		if (m_showInventory)
		{
			if (m_showCharacterInfo)
			{
				m_showCharacterInfo = false;
				m_characterInfo.hide();
			}
			m_inventory.show();
		}
		else
		{
			m_inventory.hide();
		}
	}

	if (m_showInventory)
	{
		m_inventory.update();
	}
}

void LevelInterface::setSpellManager(SpellManager* spellManager)
{
	// use this spell manager for the interface bar.
	delete m_spellSelection;
	m_spellSelection = new SpellSelection(spellManager);
}
