#include "Map/MapInterface.h"
#include "Screens/MapScreen.h"

MapInterface::MapInterface(WorldScreen* screen) : WorldInterface(screen) {
	loadGuiSidebar();
	loadMapSidebar();
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(screen, &m_core->getTotalAttributes());
	m_spellbook = new Spellbook(m_core, true);
	m_questLog = new QuestLog(m_core);
	m_mapOverlay = new MapOverlay(dynamic_cast<MapScreen*>(screen));
}

MapInterface::~MapInterface() {
	delete m_guiSidebar;
	delete m_mapSidebar;
	delete m_inventory;
	delete m_characterInfo;
	delete m_spellbook;
	delete m_questLog;
	delete m_mapOverlay;
}

