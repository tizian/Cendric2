#include "Map/MapInterface.h"
#include "Screens/MapScreen.h"

MapInterface::MapInterface(WorldScreen* screen) : WorldInterface(screen) {
	loadGuiSidebar();
	loadMapSidebar(false);
	m_quickSlotBar = new QuickSlotBar(this);
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(screen);
	m_spellbook = new Spellbook(this, true);
	m_questLog = new QuestLog(this);
	m_mapOverlay = new MapOverlay(screen, m_mapSidebar);

	m_quickSlotBar->hide();
}

MapInterface::~MapInterface() {
	delete m_guiSidebar;
	delete m_mapSidebar;
	delete m_inventory;
	delete m_characterInfo;
	delete m_spellbook;
	delete m_questLog;
	delete m_mapOverlay;
	delete m_quickSlotBar;
}

void MapInterface::showQuickslotBar(bool show) {
	if (show) {
		m_quickSlotBar->show();
	}
	else {
		m_quickSlotBar->hide();
	}
}

