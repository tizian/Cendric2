#include "GUI/WorldInterface.h"
#include "Screens/WorldScreen.h"

WorldInterface::WorldInterface(WorldScreen* screen) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
}

WorldInterface::~WorldInterface() {
}

void WorldInterface::render(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());

	m_characterInfo->render(target);
	m_spellbook->render(target);
	m_questLog->render(target);
	m_inventory->render(target);
}

void WorldInterface::update(const sf::Time& frameTime) {
	updateInventory(frameTime);
	updateSpellbook(frameTime);
	updateQuestLog(frameTime);
	updateCharacterInfo(frameTime);
}

void WorldInterface::hideAll() {
	m_characterInfo->hide();
	m_spellbook->hide();
	m_questLog->hide();
	m_inventory->hide();
}

void WorldInterface::reloadInventory(const std::string& changedItemID) {
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

void WorldInterface::updateCharacterInfo(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo)) {
		if (!m_characterInfo->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, ResourceID::Sound_gui_openwindow);
			m_characterInfo->show();
		}
		else {
			m_characterInfo->hide();
		}
	}
	else if (m_characterInfo->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_characterInfo->hide();
		g_inputController->lockAction();
	}

	m_characterInfo->update(frameTime);
}

void WorldInterface::updateSpellbook(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Spellbook)) {
		if (!m_spellbook->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, ResourceID::Sound_gui_openwindow);
			m_spellbook->show();
		}
		else {
			m_spellbook->hide();
		}
	}
	else if (m_spellbook->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_spellbook->hide();
		g_inputController->lockAction();
	}

	m_spellbook->update(frameTime);
}

void WorldInterface::updateInventory(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Inventory)) {
		if (!m_inventory->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, ResourceID::Sound_gui_openwindow);
			m_inventory->show();
		}
		else {
			m_inventory->hide();
		}
	}
	else if (m_inventory->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_inventory->hide();
		g_inputController->lockAction();
	}

	m_inventory->update(frameTime);
}

void WorldInterface::updateQuestLog(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Journal)) {
		if (!m_questLog->isVisible()) {
			hideAll();
			g_resourceManager->playSound(m_openSound, ResourceID::Sound_gui_openwindow);
			m_questLog->show();
		}
		else {
			m_questLog->hide();
		}
	}
	else if (m_questLog->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_questLog->hide();
		g_inputController->lockAction();
	}

	m_questLog->update(frameTime);
}

CharacterCore* WorldInterface::getCore() const {
	return m_core;
}

Screen* WorldInterface::getScreen() const {
	return m_screen;
}

Inventory* WorldInterface::getInventory() const {
	return m_inventory;
}