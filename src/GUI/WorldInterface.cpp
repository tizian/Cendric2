#include "GUI/WorldInterface.h"
#include "GUI/GUITabButton.h"
#include "GUI/WeaponWindow.h"
#include "Screens/WorldScreen.h"
#include "GlobalResource.h"

WorldInterface::WorldInterface(WorldScreen* screen) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
}

void WorldInterface::render(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());

	m_mapSidebar->render(target);
	m_guiSidebar->render(target);
	m_characterInfo->render(target);
	m_spellbook->render(target);
	m_questLog->render(target);
	m_quickSlotBar->render(target);
	m_inventory->render(target);
	m_mapOverlay->render(target);
}

void WorldInterface::renderAfterForeground(sf::RenderTarget& target) {
	target.setView(target.getDefaultView());
	m_inventory->renderAfterForeground(target);
	m_questLog->renderAfterForeground(target);
	m_quickSlotBar->renderAfterForeground(target);
}

void WorldInterface::update(const sf::Time& frameTime) {
	m_guiSidebar->update(frameTime);
	m_mapSidebar->update(frameTime);
	updateGuiElement(frameTime, m_inventory, GUIElement::Inventory);
	updateGuiElement(frameTime, m_characterInfo, GUIElement::Character);
	updateGuiElement(frameTime, m_spellbook, GUIElement::Spellbook);
	updateGuiElement(frameTime, m_questLog, GUIElement::Journal);
	updateGuiElement(frameTime, m_mapOverlay, GUIElement::Map);
	m_quickSlotBar->update(frameTime);

	if (g_inputController->isKeyJustPressed(Key::Menu)) {
		g_inputController->lockAction();

		if (m_guiSidebar->isVisible()) {
			hideAll();
		}
		else {
			showGuiElement(m_selectedElement);
		}
	}
}

void WorldInterface::hideAll() {
	m_mapSidebar->hide();
	m_guiSidebar->hide();
	m_characterInfo->hide();
	m_spellbook->hide();
	m_questLog->hide();
	m_inventory->hide();
	m_mapOverlay->hide();
	for (auto go : *m_screen->getObjects(_ScreenOverlay)) {
		go->setDisposed();
	}
}

bool WorldInterface::isGuiOverlayVisible() const {
	return m_characterInfo->isVisible() ||
		m_spellbook->isVisible() ||
		m_questLog->isVisible() ||
		m_inventory->isVisible() ||
		m_mapOverlay->isVisible();
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

void WorldInterface::reloadMapWaypoints() {
	m_mapOverlay->reloadMarkers();
}

void WorldInterface::reloadLevelOverlay() {
	m_mapOverlay->notifyLevelOverlayReload();
}

void WorldInterface::jumpToQuestMarker(const std::string& questId, const std::vector<QuestMarkerData>& data) {
	showGuiElement(m_mapOverlay, GUIElement::Map);
	m_guiSidebar->setWindowSelected(false);
	m_mapSidebar->setWindowSelected(false);
	m_mapOverlay->setWindowSelected(true);
	m_mapOverlay->notifyJumpToQuest(questId, data);
}

void WorldInterface::jumpToQuestLog(const std::string& questId) {
	showGuiElement(m_questLog, GUIElement::Journal);
	m_guiSidebar->setWindowSelected(false);
	m_questLog->setWindowSelected(true);
	m_questLog->notifyJumpToQuest(questId);
}

void WorldInterface::showInventory() {
	showGuiElement(m_inventory, GUIElement::Inventory);
	m_guiSidebar->setWindowSelected(false);
	m_inventory->setWindowSelected(true);
}

void WorldInterface::notifyConsumableDrop(const SlotClone* item) const {
	m_quickSlotBar->notifyConsumableDrop(item);
}

void WorldInterface::equipConsumable(const std::string& itemID) const {
	m_quickSlotBar->equipConsumable(itemID);
}

void WorldInterface::equipConsumable(const std::string& itemID, int slotId) const {
	m_quickSlotBar->equipConsumable(itemID, slotId);
}

void WorldInterface::highlightQuickslots(bool highlight) const {
	m_quickSlotBar->highlightSlots(highlight);
}

void WorldInterface::showGuiElement(GUIElement type) {
	switch (type) {
	default:
	case GUIElement::VOID:
		return;
	case GUIElement::Character:
		 showGuiElement(m_characterInfo, type);
		 return;
	case GUIElement::Inventory:
		showGuiElement(m_inventory, type);
		return;
	case GUIElement::Spellbook:
		showGuiElement(m_spellbook, type);
		return;
	case GUIElement::Journal:
		showGuiElement(m_questLog, type);
		return;
	case GUIElement::Map:
		showGuiElement(m_mapOverlay, type);
		return;
	}
}

void WorldInterface::connectGuiElements(GUIElement type) {
	m_characterInfo->setWindowSelected(false);
	m_inventory->setWindowSelected(false);
	m_inventory->getEquipment()->setWindowSelected(false);
	m_questLog->setWindowSelected(false);
	m_mapSidebar->setWindowSelected(false);
	m_mapOverlay->setWindowSelected(false);
	m_spellbook->setWindowSelected(false);
	m_spellbook->getWeaponWindow()->setWindowSelected(false);

	m_guiSidebar->setRightWindow(nullptr);
	m_guiSidebar->setWindowSelected(true);

	switch (type) {
	default:
	case GUIElement::VOID:
		return;
	case GUIElement::Character:
		m_guiSidebar->setRightWindow(m_characterInfo);
		return;
	case GUIElement::Inventory:
		m_guiSidebar->setRightWindow(m_inventory->getEquipment());
		m_inventory->getEquipment()->setRightWindow(m_inventory);
		return;
	case GUIElement::Spellbook:
		m_guiSidebar->setRightWindow(m_spellbook);
		m_spellbook->setRightWindow(m_spellbook->getWeaponWindow());
		return;
	case GUIElement::Journal:
		m_guiSidebar->setRightWindow(m_questLog);
		return;
	case GUIElement::Map:
		m_guiSidebar->setRightWindow(m_mapOverlay);
		m_mapOverlay->setRightWindow(m_mapSidebar);
		return;
	}
}

template<typename G>
void WorldInterface::showGuiElement(G* guiElement, GUIElement type) {
	hideAll();
	g_resourceManager->playSound(GlobalResource::SOUND_GUI_OPENWINDOW);
	guiElement->show();
	m_guiSidebar->show(static_cast<int>(type));
	connectGuiElements(type);
	m_selectedElement = type;
}

template<typename G>
void WorldInterface::updateGuiElement(const sf::Time& frameTime, G* guiElement, GUIElement type) {
	if (g_inputController->isKeyJustPressed(getKeyFromGuiElement(type))) {
		if (!guiElement->isVisible()) {
			showGuiElement(guiElement, type);
		}
		else {
			guiElement->hide();
			m_guiSidebar->hide();
		}
	}
	else if (guiElement->isVisible() && g_inputController->isKeyJustPressed(Key::Escape)) {
		guiElement->hide();
		m_guiSidebar->hide();
		g_inputController->lockAction();
	}
	else if (!guiElement->isVisible() && m_guiSidebar->isVisible() && m_guiSidebar->getSelectedElement() == static_cast<int>(type)) {
		showGuiElement(guiElement, type);
	}

	guiElement->update(frameTime);
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

void WorldInterface::loadGuiSidebar() {
	delete m_guiSidebar;
	int n = 5;
	m_guiSidebar = new GUITabBar(this, n);
	m_guiSidebar->setWindowSelected(true);

	const sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ICONS);

	for (int i = 0; i < n; ++i) {
		m_guiSidebar->setButtonTexture(i, tex, i * GUITabButton::ICON_SIZE);
		m_guiSidebar->setButtonText(i, EnumNames::getShortKeyboardKeyName(
			g_resourceManager->getConfiguration().mainKeyMap[getKeyFromGuiElement(static_cast<GUIElement>(i))]));
	}

	int height = n * GUITabButton::SIZE + (n - 1) * GUITabBar::BUTTON_MARGIN + 4 * static_cast<int>(GUIConstants::TEXT_OFFSET);
	m_guiSidebar->setPosition(sf::Vector2f(GUIConstants::LEFT_BAR,
		0.5f * (WINDOW_HEIGHT - height)));
}

void WorldInterface::loadMapSidebar(bool isLevel) {
	delete m_mapSidebar;

	auto const& tilesExplored = m_core->getData().tilesExplored;

	int n = static_cast<int>(tilesExplored.size());
	int size = isLevel ? n + 1 : n;
	m_mapSidebar = new GUITabBar(this, size);

	int height = size * GUITabButton::SIZE + (size - 1) * GUITabBar::BUTTON_MARGIN + 4 * static_cast<int>(GUIConstants::TEXT_OFFSET);
	m_mapSidebar->setPosition(sf::Vector2f(WINDOW_WIDTH - (GUIConstants::LEFT_BAR + GUITabBar::WIDTH),
		0.5f * (WINDOW_HEIGHT - height)));
}

Key WorldInterface::getKeyFromGuiElement(GUIElement e) {
	switch (e) {
	default:
	case GUIElement::VOID:
		return Key::VOID;
	case GUIElement::Character:
		return Key::CharacterInfo;
	case GUIElement::Inventory:
		return Key::Inventory;
	case GUIElement::Spellbook:
		return Key::Spellbook;
	case GUIElement::Journal:
		return Key::Journal;
	case GUIElement::Map:
		return Key::Map;
	}
}