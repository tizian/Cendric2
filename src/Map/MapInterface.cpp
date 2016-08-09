#include "Map/MapInterface.h"
#include "Screens/MapScreen.h"

MapInterface::MapInterface(WorldScreen* screen) : WorldInterface(screen) {
	m_sidebar = new GUITabBar(this);
	m_inventory = new Inventory(this);
	m_characterInfo = new CharacterInfo(m_core, &m_core->getTotalAttributes());
	m_spellbook = new Spellbook(m_core, true);
	m_questLog = new QuestLog(m_core);
	m_mapOverlay = new MapOverlay(dynamic_cast<MapScreen*>(screen));
}

MapInterface::~MapInterface() {
	delete m_sidebar;
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

void MapInterface::hideAll() {
	m_mapOverlay->hide();
	WorldInterface::hideAll();
}

bool MapInterface::isGuiOverlayVisible() const {
	return WorldInterface::isGuiOverlayVisible() ||
		m_mapOverlay->isVisible();
}

void MapInterface::updateMapOverlay(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Map)) {
		if (!m_mapOverlay->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
			m_mapOverlay->show();
			m_sidebar->show(GUIElement::Map);
		}
		else {
			m_mapOverlay->hide();
			m_sidebar->hide();
		}
	}
	else if (m_mapOverlay->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_mapOverlay->hide();
		m_sidebar->hide();
		g_inputController->lockAction();
	}
	else if (!m_mapOverlay->isVisible() && m_sidebar->getActiveElement() == GUIElement::Map) {
		hideAll();
		g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
		m_mapOverlay->show();
		m_sidebar->show(GUIElement::Map);
	}

	m_mapOverlay->update(frameTime);
}
