#include "GUI/WorldInterface.h"
#include "Screens/WorldScreen.h"
#include "GlobalResource.h"

WorldInterface::WorldInterface(WorldScreen* screen) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
}

WorldInterface::~WorldInterface() {
}

void WorldInterface::render(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());

	m_sidebar->render(target);
	m_characterInfo->render(target);
	m_spellbook->render(target);
	m_questLog->render(target);
	m_inventory->render(target);
}

void WorldInterface::renderAfterForeground(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());
	m_inventory->renderAfterForeground(target);
}

void WorldInterface::update(const sf::Time& frameTime) {
	updateSidebar(frameTime);
	updateInventory(frameTime);
	updateSpellbook(frameTime);
	updateQuestLog(frameTime);
	updateCharacterInfo(frameTime);
}

void WorldInterface::hideAll() {
	m_sidebar->hide();
	m_characterInfo->hide();
	m_spellbook->hide();
	m_questLog->hide();
	m_inventory->hide();
	for (auto go : *m_screen->getObjects(GameObjectType::_ScreenOverlay)) {
		go->setDisposed();
	}
}

bool WorldInterface::isGuiOverlayVisible() const {
	return m_characterInfo->isVisible() || 
	m_spellbook->isVisible() || 
	m_questLog->isVisible() ||
	m_inventory->isVisible();
}

void WorldInterface::reloadInventory(const std::string& changedItemID) {
	if (changedItemID.empty()) {
		m_inventory->reload();
		return;
	}
	m_inventory->notifyChange(changedItemID);
}

void WorldInterface::reloadQuestLog() {
	m_questLog->reload();
}

void WorldInterface::reloadCharacterInfo() {
	m_characterInfo->notifyChange();
}

void WorldInterface::reloadSpellBook() {
	m_spellbook->reload();
}

void WorldInterface::updateSidebar(const sf::Time& frameTime) {
	m_sidebar->update(frameTime);
}

void WorldInterface::updateCharacterInfo(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo)) {
		if (!m_characterInfo->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
			m_characterInfo->show();
			m_sidebar->show(GUIElement::Character);
		}
		else {
			m_characterInfo->hide();
			m_sidebar->hide();
		}
	}
	else if (m_characterInfo->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_characterInfo->hide();
		m_sidebar->hide();
		g_inputController->lockAction();
	}
	else if (!m_characterInfo->isVisible() && m_sidebar->getActiveElement() == GUIElement::Character) {
		hideAll();
		g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
		m_characterInfo->show();
		m_sidebar->show(GUIElement::Character);
	}

	m_characterInfo->update(frameTime);
}

void WorldInterface::updateSpellbook(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Spellbook)) {
		if (!m_spellbook->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
			m_spellbook->show();
			m_sidebar->show(GUIElement::Spellbook);
		}
		else {
			m_spellbook->hide();
			m_sidebar->hide();
		}
	}
	else if (m_spellbook->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_spellbook->hide();
		m_sidebar->hide();
		g_inputController->lockAction();
	}
	else if (!m_spellbook->isVisible() && m_sidebar->getActiveElement() == GUIElement::Spellbook) {
		hideAll();
		g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
		m_spellbook->show();
		m_sidebar->show(GUIElement::Spellbook);
	}

	m_spellbook->update(frameTime);
}

void WorldInterface::updateInventory(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Inventory)) {
		if (!m_inventory->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
			m_inventory->show();
			m_sidebar->show(GUIElement::Inventory);
		}
		else {
			m_inventory->hide();
			m_sidebar->hide();
		}
	}
	else if (m_inventory->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_inventory->hide();
		m_sidebar->hide();
		g_inputController->lockAction();
	}
	else if (!m_inventory->isVisible() && m_sidebar->getActiveElement() == GUIElement::Inventory) {
		hideAll();
		g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
		m_inventory->show();
		m_sidebar->show(GUIElement::Inventory);
	}

	m_inventory->update(frameTime);
}

void WorldInterface::updateQuestLog(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Journal)) {
		if (!m_questLog->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
			m_questLog->show();
			m_sidebar->show(GUIElement::Journal);
		}
		else {
			m_questLog->hide();
			m_sidebar->hide();
		}
	}
	else if (m_questLog->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_questLog->hide();
		m_sidebar->hide();
		g_inputController->lockAction();
	}
	else if (!m_questLog->isVisible() && m_sidebar->getActiveElement() == GUIElement::Journal) {
		hideAll();
		g_resourceManager->playSound(m_openSound, GlobalResource::SOUND_GUI_OPENWINDOW);
		m_questLog->show();
		m_sidebar->show(GUIElement::Journal);
	}

	m_questLog->update(frameTime);
}

CharacterCore* WorldInterface::getCore() const {
	return m_core;
}

WorldScreen* WorldInterface::getScreen() const {
	return m_screen;
}

Inventory* WorldInterface::getInventory() const {
	return m_inventory;
}