#include "GUI/Inventory.h"
#include "Level/LevelMainCharacter.h"
#include "Level/LevelInterface.h"
#include "Map/MapInterface.h"
#include "Map/MerchantInterface.h"
#include "GUI/GUIConstants.h"
#include "GUI/SlotClone.h"

Inventory::Inventory(LevelInterface* _interface) {
	m_levelInterface = _interface;
	m_core = _interface->getCore();

	init();
}

Inventory::Inventory(MapInterface* _interface) {
	m_mapInterface = _interface;
	m_core = _interface->getCore();

	init();
}

void Inventory::init() {
	m_selectedSlotId.first = "";
	m_selectedSlotId.second = ItemType::VOID;
	// init window
	sf::FloatRect box(INVENTORY_LEFT, GUIConstants::TOP, INVENTORY_WIDTH, INVENTORY_HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::FANCY,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	m_descriptionWindow = new ItemDescriptionWindow();

	// init text
	m_selectedTabText.setPosition(sf::Vector2f(INVENTORY_LEFT + GUIConstants::TEXT_OFFSET, GUIConstants::TOP + GUIConstants::TEXT_OFFSET));
	m_selectedTabText.setColor(COLOR_WHITE);
	m_selectedTabText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	m_goldText.setPosition(sf::Vector2f(INVENTORY_LEFT + GUIConstants::TEXT_OFFSET, GUIConstants::TOP + INVENTORY_HEIGHT - GUIConstants::TEXT_OFFSET - GUIConstants::CHARACTER_SIZE_S));
	m_goldText.setColor(COLOR_WHITE);
	m_goldText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	// fill the helper map
	m_typeMap.insert({
		{ ItemType::Consumable, &m_consumableItems },
		{ ItemType::Permanent, &m_consumableItems },
		{ ItemType::Misc, &m_miscItems },
		{ ItemType::Document, &m_documentItems },
		{ ItemType::Quest, &m_questItems },
		{ ItemType::Equipment_back, &m_equipmentItems },
		{ ItemType::Equipment_body, &m_equipmentItems },
		{ ItemType::Equipment_head, &m_equipmentItems },
		{ ItemType::Equipment_neck, &m_equipmentItems },
		{ ItemType::Equipment_ring_1, &m_equipmentItems },
		{ ItemType::Equipment_ring_2, &m_equipmentItems },
		{ ItemType::Equipment_weapon, &m_equipmentItems },
	});

	// tabbar
	int nTabs = 5;
	float width = nTabs * BUTTON_SIZE.x;
	float height = BUTTON_SIZE.y;
	float x = INVENTORY_LEFT + 0.5f * (INVENTORY_WIDTH - width);
	float y = GUIConstants::TOP + GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + 2 * MARGIN;

	m_tabBar = new TexturedTabBar(sf::FloatRect(x, y, width, height), nTabs);

	int textureOffset = 0;
	for (int i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setTexture(g_resourceManager->getTexture(ResourceID::Texture_inventorytabs), sf::IntRect(textureOffset, 0, 60, 35));
		textureOffset += 60;
	}

	selectTab(ItemType::Equipment_weapon);

	m_equipment = new InventoryEquipment(m_core, m_levelInterface != nullptr);
	reload();
}

Inventory::~Inventory() {
	delete m_window;
	delete m_descriptionWindow;
	delete m_documentWindow;
	delete m_equipment;
	delete m_currentClone;
	delete m_tabBar;
	clearAllSlots();
}

void Inventory::clearAllSlots() {
	m_consumableItems.clear();
	m_equipmentItems.clear();
	m_questItems.clear();
	m_documentItems.clear();
	m_miscItems.clear();
	m_selectedSlotId.first = "";
}

InventorySlot* Inventory::getSelectedSlot() const {
	if (m_selectedSlotId.first.empty()) return nullptr;
	if (m_selectedSlotId.second != ItemType::VOID) {
		return m_equipment->getSelectedSlot(m_selectedSlotId.second);
	}
	if (m_typeMap.find(m_currentTab) == m_typeMap.end()) return nullptr;
	if (m_typeMap.at(m_currentTab)->find(m_selectedSlotId.first) == m_typeMap.at(m_currentTab)->end()) return nullptr;
	return &m_typeMap.at(m_currentTab)->at(m_selectedSlotId.first);
}

void Inventory::deselectCurrentSlot() {
	InventorySlot* slot = getSelectedSlot();
	m_selectedSlotId.first = "";
	m_selectedSlotId.second = ItemType::VOID;
	m_descriptionWindow->hide();
	if (slot != nullptr) {
		slot->deselect();
	}
}

void Inventory::notifyChange(const std::string& itemID) {
	if (itemID.compare("gold") == 0) {
		reloadGold();
		return;
	}
	ItemBean bean = g_databaseManager->getItemBean(itemID);
	if (bean.status != BeanStatus::Filled) return;
	if (m_typeMap.find(bean.item_type) == m_typeMap.end()) return;

	std::map<std::string, InventorySlot>* tab = m_typeMap.at(bean.item_type);

	// search for the slot
	if (tab->find(bean.item_id) != tab->end()) {
		if (m_core->getData().items.find(itemID) == m_core->getData().items.end()) {
			// the item was removed. check if it is selected.
			if (m_selectedSlotId.first.compare(bean.item_id) == 0) {
				deselectCurrentSlot();
			}
			tab->erase(bean.item_id);
			calculateSlotPositions(*(m_typeMap.at(bean.item_type)));
		}
		else {
			tab->at(bean.item_id).setAmount(m_core->getData().items.at(itemID));
		}
		return;
	}
	
	// the slot for that item has not been found. The slot is added with the current amount in the core
	if (m_core->getData().items.find(itemID) == m_core->getData().items.end()) return;

	(*tab).insert({ bean.item_id, InventorySlot(Item(itemID), m_core->getData().items.at(itemID)) });

	calculateSlotPositions(*tab);
}

void Inventory::handleMapRightClick(InventorySlot* clicked) {
	if (m_mapInterface == nullptr || clicked == nullptr) return;
	if (m_merchantInterface != nullptr) {
		m_merchantInterface->sellItem(clicked->getItem());
		return;
	}
	if (clicked->getItemType() == ItemType::Document)
		showDocument(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Permanent)
		dynamic_cast<WorldScreen*>(m_mapInterface->getScreen())->notifyPermanentItemConsumed(clicked->getItem());
}

void Inventory::handleLevelRightClick(InventorySlot* clicked) {
	if (m_levelInterface == nullptr || clicked == nullptr) return;
	if (clicked->getItemType() == ItemType::Consumable)
		m_levelInterface->consumeItem(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Document)
		showDocument(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Permanent)
		m_levelInterface->getScreen()->setTooltipText("CannotConsumePermanentInLevel", COLOR_BAD, true);
}

void Inventory::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	// check whether an item was selected
	for (auto& it : *(m_typeMap.at(m_currentTab))) {
		it.second.update(frameTime);
		if (it.second.isClicked()) {
			selectSlot(it.second.getItemID(), ItemType::VOID);
			return;
		}
		if (it.second.isRightClicked()) {
			handleLevelRightClick(&it.second);
			handleMapRightClick(&it.second);
			break;
		}
	}

	m_tabBar->update(frameTime);
	int activeIndex = m_tabBar->getActiveTabIndex();
	ItemType type;
	if (activeIndex == 0) {
		type = ItemType::Equipment_weapon;
	}
	else if (activeIndex == 1) {
		type = ItemType::Consumable;
	}
	else if (activeIndex == 2) {
		type = ItemType::Document;
	}
	else if (activeIndex == 3) {
		type = ItemType::Quest;
	}
	else if (activeIndex == 4) {
		type = ItemType::Misc;
	}

	if (m_tabBar->getTabButton(activeIndex)->isClicked() && m_currentTab != type) {
		selectTab(type);
	}

	// update equipment part
	m_equipment->update(frameTime);
	InventorySlot* eqSlot = m_equipment->getSelectedSlot();
	if (eqSlot != nullptr) {
		selectSlot(eqSlot->getItemID(), eqSlot->getItemType());
	}

	handleDragAndDrop();

	if (m_equipment->requiresReload()) {
		reload();
	}
}

void Inventory::selectSlot(const std::string& selectedSlotId, ItemType type) {
	if (selectedSlotId.empty()) {
		deselectCurrentSlot();
		return;
	}
	m_hasDraggingStarted = true;
	m_isEquipmentSlotDragged = type != ItemType::VOID;
	m_startMousePosition = g_inputController->getDefaultViewMousePosition();

	if (selectedSlotId.compare(m_selectedSlotId.first) == 0) return;
	
	deselectCurrentSlot();
	m_selectedSlotId.first = selectedSlotId;
	m_selectedSlotId.second = type;
	InventorySlot* selectedSlot = getSelectedSlot();
	hideDocument();
	if (selectedSlot != nullptr) {
		selectedSlot->select();
		showDescription(selectedSlot->getItem());
	}
}

void Inventory::removeEquipmentItem() {
	if (m_window->getBoundingBox()->intersects(*m_currentClone->getBoundingBox())) {
		const InventorySlot *is = static_cast<const InventorySlot *>(m_currentClone->getOriginalSlot());
		m_core->equipItem("", is->getItemType());
		reload();
	}
}

void Inventory::handleMapDrag() {
	if (m_mapInterface == nullptr) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot != nullptr && !m_isEquipmentSlotDragged) {
		m_equipment->highlightEquipmentSlot(selectedSlot->getItemType(), true);
	}
}

void Inventory::handleLevelDrag() {
	if (m_levelInterface == nullptr) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot != nullptr && selectedSlot->getItemType() == ItemType::Consumable) {
		m_levelInterface->highlightQuickslots(true);
	}
}

void Inventory::handleMapDrop() {
	if (m_mapInterface == nullptr || m_currentClone == nullptr) return;
	if (m_isEquipmentSlotDragged) {
		removeEquipmentItem();
	}
	else {
		if (m_equipment->notifyEquipmentDrop(m_currentClone)) {
			reload();
		}
		const InventorySlot *is = static_cast<const InventorySlot *>(m_currentClone->getOriginalSlot());
		m_equipment->highlightEquipmentSlot(is->getItemType(), false);
	}
}

void Inventory::handleLevelDrop() {
	if (m_levelInterface == nullptr || m_currentClone == nullptr) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot == nullptr) return;
	ItemType type = selectedSlot->getItemType();
	if (type == ItemType::Consumable) {
		m_levelInterface->notifyConsumableDrop(m_currentClone);
		m_levelInterface->highlightQuickslots(false);
	}
	else if (m_isEquipmentSlotDragged 
		|| type == ItemType::Equipment_back 
		|| type == ItemType::Equipment_body
		|| type == ItemType::Equipment_head
		|| type == ItemType::Equipment_neck
		|| type == ItemType::Equipment_ring_1
		|| type == ItemType::Equipment_ring_2
		|| type == ItemType::Equipment_weapon) {
		m_levelInterface->getScreen()->setTooltipText("CannotEquipInLevel", COLOR_BAD, true);
	}
}

void Inventory::handleDragAndDrop() {
	if (!m_hasDraggingStarted) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (!(g_inputController->isMousePressedLeft())) {
		if (selectedSlot != nullptr) {
			selectedSlot->activate();
			handleLevelDrop();
			handleMapDrop();
		}
		delete m_currentClone;
		m_currentClone = nullptr;
		m_hasDraggingStarted = false;
		m_isDragging = false;
		return;
	}
	sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
	if (!m_isDragging) {
		if (DRAG_DISTANCE < std::sqrt(
			(mousePos.x - m_startMousePosition.x) * (mousePos.x - m_startMousePosition.x) +
			(mousePos.y - m_startMousePosition.y) * (mousePos.y - m_startMousePosition.y))) {
			m_isDragging = true;
			delete m_currentClone;
			m_currentClone = new SlotClone(selectedSlot);
			m_currentClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIZE / 2.f, InventorySlot::SIZE / 2.f));
			selectedSlot->deactivate();
			handleLevelDrag();
			handleMapDrag();
		}
	}
	else {
		m_currentClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIZE / 2.f, InventorySlot::SIZE / 2.f));
	}
}

bool Inventory::isVisible() const {
	return m_isVisible;
}

void Inventory::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_goldText);
	target.draw(m_selectedTabText);
	for (auto& it : *(m_typeMap.at(m_currentTab))) {
		it.second.render(target);
		// it.second.renderAfterForeground(target); // uncomment for debug box
	}

	m_tabBar->render(target);

	m_descriptionWindow->render(target);
	if (m_documentWindow != nullptr) {
		m_documentWindow->render(target);
	}

	m_equipment->render(target);
	if (m_currentClone != nullptr) {
		m_currentClone->render(target);
	}
}

void Inventory::showDocument(const Item& item) {
	delete m_documentWindow;
	m_documentWindow = new DocumentDescriptionWindow(item);

	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x + MARGIN + m_window->getSize().x,
		m_window->getPosition().y);
	if (m_descriptionWindow->isVisible()) {
		pos.x += ItemDescriptionWindow::WIDTH + MARGIN;
	}
	m_documentWindow->setPosition(pos);
}

void Inventory::showDescription(const Item& item) {
	m_descriptionWindow->load(item);
	m_descriptionWindow->show();
	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x + MARGIN + m_window->getSize().x,
		m_window->getPosition().y);
	m_descriptionWindow->setPosition(pos);
	if (m_documentWindow != nullptr) {
		pos.x += ItemDescriptionWindow::WIDTH + MARGIN;
		m_documentWindow->setPosition(pos);
	}
}

void Inventory::hideDescription() {
	m_descriptionWindow->hide();
	if (m_documentWindow != nullptr) {
		m_documentWindow->setPosition(
			m_documentWindow->getPosition() -
			sf::Vector2f(ItemDescriptionWindow::WIDTH + MARGIN, 0.f));
	}
}

void Inventory::hideDocument() {
	delete m_documentWindow;
	m_documentWindow = nullptr;
}

void Inventory::selectTab(ItemType type) {
	hideDescription();
	hideDocument();
	deselectCurrentSlot();
	m_currentTab = type;
	switch (type) {
	case ItemType::Equipment_weapon:
		m_selectedTabText.setString(g_textProvider->getText("Equipment"));
		break;
	case ItemType::Consumable:
		m_selectedTabText.setString(g_textProvider->getText("Consumables"));
		break;
	case ItemType::Document:
		m_selectedTabText.setString(g_textProvider->getText("Documents"));
		break;
	case ItemType::Quest:
		m_selectedTabText.setString(g_textProvider->getText("QuestItems"));
		break;
	case ItemType::Misc:
		m_selectedTabText.setString(g_textProvider->getText("Miscellaneous"));
		break;
	default:
		break;
	}
	// center text
	m_selectedTabText.setPosition(
		m_window->getPosition().x +
		INVENTORY_WIDTH / 2 -
		m_selectedTabText.getLocalBounds().width / 2, m_selectedTabText.getPosition().y);
}

void Inventory::reloadGold() {
	std::string gold = "";
	gold.append(g_textProvider->getText("Gold"));
	gold.append(": ");
	gold.append(std::to_string(m_core->getData().gold));
	gold.append("\n\n");
	m_goldText.setString(gold);
}

void Inventory::reload() {
	// reload gold
	reloadGold();

	// reload items
	clearAllSlots();
	hideDescription();
	hideDocument();
	m_core->loadItems();
	for (auto& itemData : m_core->getData().items) {
		const Item* item = m_core->getItem(itemData.first);
		if (item == nullptr || m_typeMap.find(item->getType()) == m_typeMap.end()) continue;
		m_typeMap.at(item->getType())->insert({ item->getID(), InventorySlot(*item, itemData.second) });
	}

	calculateSlotPositions(m_consumableItems);
	calculateSlotPositions(m_equipmentItems);
	calculateSlotPositions(m_questItems);
	calculateSlotPositions(m_documentItems);
	calculateSlotPositions(m_miscItems);

	// reload equipment
	m_equipment->reload();
}

void Inventory::calculateSlotPositions(std::map<std::string, InventorySlot>& slots) {
	float yOffset = GUIConstants::TOP + 2 * GUIConstants::TEXT_OFFSET + InventorySlot::ICON_OFFSET + GUIConstants::CHARACTER_SIZE_M + 2 * MARGIN + BUTTON_SIZE.y;
	float xOffset = INVENTORY_LEFT + GUIConstants::TEXT_OFFSET + InventorySlot::ICON_OFFSET;
	int y = 1;
	int x = 1;
	for (auto& it : slots) {
		it.second.setPosition(sf::Vector2f(xOffset, yOffset));
		if (x + 1 > SLOT_COUNT_X) {
			x = 1;
			xOffset = INVENTORY_LEFT + GUIConstants::TEXT_OFFSET + InventorySlot::ICON_OFFSET;
			y++;
			yOffset += MARGIN + InventorySlot::SIZE;
		}
		else {
			x++;
			xOffset += MARGIN + InventorySlot::SIZE;
		}
	}
}

void Inventory::show() {
	m_isVisible = true;
	m_equipment->show();
}

void Inventory::hide() {
	m_isVisible = false;
	m_equipment->hide();
	delete m_currentClone;
	m_currentClone = nullptr;
	m_isDragging = false;
	m_hasDraggingStarted = false;
}

void Inventory::startTrading(MerchantInterface* _interface) {
	m_merchantInterface = _interface;
	show();
}

void Inventory::stopTrading() {
	m_merchantInterface = nullptr;
	hide();
}