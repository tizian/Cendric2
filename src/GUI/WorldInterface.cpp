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
}

void WorldInterface::hideAll() {
	m_mapSidebar->hide();
	m_guiSidebar->hide();
	m_characterInfo->hide();
	m_spellbook->hide();
	m_questLog->hide();
	m_inventory->hide();
	m_mapOverlay->hide();
	for (auto go : *m_screen->getObjects(GameObjectType::_ScreenOverlay)) {
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

void WorldInterface::reloadMapWaypoints() {
	m_mapOverlay->reloadWaypoints();
}

void WorldInterface::reloadLevelOverlay() {
	m_mapOverlay->notifyLevelOverlayReload();
}

void WorldInterface::notifyConsumableDrop(const SlotClone* item) {
	m_quickSlotBar->notifyConsumableDrop(item);
}

void WorldInterface::equipConsumable(const std::string& itemID) {
	m_quickSlotBar->equipConsumable(itemID);
}

void WorldInterface::highlightQuickslots(bool highlight) {
	m_quickSlotBar->highlightSlots(highlight);
}

template<typename G>
void WorldInterface::updateGuiElement(const sf::Time& frameTime, G* guiElement, GUIElement type) {
	if (g_inputController->isKeyJustPressed(getKeyFromGuiElement(type))) {
		if (!guiElement->isVisible()) {
			hideAll();
			g_resourceManager->playSound(GlobalResource::SOUND_GUI_OPENWINDOW);
			guiElement->show();
			m_guiSidebar->show(static_cast<int>(type));
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
	else if (!guiElement->isVisible() && m_guiSidebar->getActiveElement() == static_cast<int>(type)) {
		hideAll();
		g_resourceManager->playSound(GlobalResource::SOUND_GUI_OPENWINDOW);
		guiElement->show();
		m_guiSidebar->show(static_cast<int>(type));
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

	const sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ICONS);

	for (int i = 0; i < n; ++i) {
		m_guiSidebar->setButtonTexture(i, tex, i * GUITabButton::ICON_SIZE);
		m_guiSidebar->setButtonText(i, EnumNames::getKeyboardKeyName(
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
