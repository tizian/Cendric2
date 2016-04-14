#include "Map/MapInterface.h"
#include "Screens/MapScreen.h"

MapInterface::MapInterface(WorldScreen* screen) : WorldInterface(screen) {
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(m_core, &m_core->getTotalAttributes());
	m_spellbook = new Spellbook(m_core, true);
	m_questLog = new QuestLog(m_core);
	m_mapOverlay = new MapOverlay(dynamic_cast<MapScreen*>(screen));
}

MapInterface::~MapInterface() {
	delete m_inventory;
	delete m_characterInfo;
	delete m_spellbook;
	delete m_questLog;
	delete m_mapOverlay;
}

void MapInterface::update(const sf::Time& frameTime) {
	if (m_mapOverlay->isVisible() && (g_inputController->isKeyJustPressed(Key::Inventory) ||
		g_inputController->isKeyJustPressed(Key::CharacterInfo) ||
		g_inputController->isKeyJustPressed(Key::Journal) ||
		g_inputController->isKeyJustPressed(Key::Spellbook)))
	{
		m_mapOverlay->hide();
	}
	updateMapOverlay(frameTime);
	WorldInterface::update(frameTime);
}

void MapInterface::render(sf::RenderTarget& target) {
	WorldInterface::render(target);

	m_mapOverlay->render(target);
}

void MapInterface::updateMapOverlay(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Map)) {
		if (!m_mapOverlay->isVisible()) {
			if (m_inventory->isVisible()) m_inventory->hide();
			if (m_spellbook->isVisible()) m_spellbook->hide();
			if (m_questLog->isVisible()) m_questLog->hide();
			if (m_characterInfo->isVisible()) m_characterInfo->hide();

			m_mapOverlay->show();
		}
		else {
			m_mapOverlay->hide();
		}
	}
	else if (m_mapOverlay->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_mapOverlay->hide();
		g_inputController->lockAction();
	}
	m_mapOverlay->update(frameTime);
}
