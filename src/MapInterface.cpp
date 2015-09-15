#include "MapInterface.h"

MapInterface::MapInterface(GameScreen* screen) : GameInterface(screen)
{
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(&m_core->getTotalAttributes());
	m_spellbook = new Spellbook(m_core, true);
	m_questLog = new QuestLog(m_core);
}

MapInterface::~MapInterface()
{
	delete m_inventory;
	delete m_characterInfo;
	delete m_spellbook;
	delete m_questLog;
}
