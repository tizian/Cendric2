#include "GUI/Inventory.h"
#include "GUI/GUIConstants.h"
#include "GUI/SlotClone.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "Level/LevelMainCharacter.h"
#include "Level/LevelInterface.h"
#include "Map/MapInterface.h"
#include "Map/MerchantInterface.h"
#include "GlobalResource.h"
#include "World/Item.h"

const int Inventory::SLOT_COUNT_X = 5;
const int Inventory::SLOT_COUNT_Y = 6;

const float Inventory::ICON_MARGIN = 8.f;
const float Inventory::WINDOW_MARGIN = 6.f;

const sf::Vector2f Inventory::BUTTON_SIZE = sf::Vector2f(50.f + 10.f, 35.f + 10.f);  // the 10.f is for the margin, 50x35 is the size of the texture

const float Inventory::SCROLL_WINDOW_LEFT = GUIConstants::TEXT_OFFSET;
const float Inventory::SCROLL_WINDOW_TOP = GUIConstants::GUI_TABS_TOP + 2 * WINDOW_MARGIN + BUTTON_SIZE.y;
const float Inventory::SCROLL_WINDOW_WIDTH = SLOT_COUNT_X * InventorySlot::SIZE + (SLOT_COUNT_X - 1) * ICON_MARGIN + 4 * WINDOW_MARGIN + ScrollBar::WIDTH;
const float Inventory::SCROLL_WINDOW_HEIGHT = SLOT_COUNT_Y * InventorySlot::SIZE + (SLOT_COUNT_Y - 1) * ICON_MARGIN + 4 * WINDOW_MARGIN;

const float Inventory::INVENTORY_LEFT = GUIConstants::LEFT + WINDOW_MARGIN + InventoryEquipment::WIDTH;
const float Inventory::INVENTORY_WIDTH = SCROLL_WINDOW_WIDTH + 2 * SCROLL_WINDOW_LEFT;

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

WorldInterface* Inventory::getInterface() const {
	if (m_mapInterface == nullptr) {
		return m_levelInterface;
	}
	return m_mapInterface;
}

void Inventory::init() {
	m_selectedSlotId.first = "";
	m_selectedSlotId.second = ItemType::VOID;
	// init window
	const sf::FloatRect box(INVENTORY_LEFT, GUIConstants::TOP, INVENTORY_WIDTH, GUIConstants::GUI_WINDOW_HEIGHT);
	m_window = new Window(box,
		GUIOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&Inventory::hide, this));

	m_descriptionWindow = new ItemDescriptionWindow();

	// init text
	m_selectedTabText.setColor(COLOR_WHITE);
	m_selectedTabText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	m_goldText.setColor(COLOR_WHITE);
	m_goldText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	m_goldSprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_PROGRESSLOG_ICONS));
	m_goldSprite.setTextureRect(sf::IntRect(0, 0, 25, 25));

	// fill the helper map
	m_typeMap.insert({
		{ ItemType::Consumable, &m_consumableItems },
		{ ItemType::Permanent, &m_consumableItems },
		{ ItemType::Misc, &m_miscItems },
		{ ItemType::Convertible, &m_miscItems },
		{ ItemType::Document, &m_documentItems },
		{ ItemType::Spell, &m_documentItems },
		{ ItemType::Quest, &m_questItems },
		{ ItemType::Key, &m_keyItems },
		{ ItemType::Equipment_back, &m_equipmentItems },
		{ ItemType::Equipment_body, &m_equipmentItems },
		{ ItemType::Equipment_head, &m_equipmentItems },
		{ ItemType::Equipment_neck, &m_equipmentItems },
		{ ItemType::Equipment_ring_1, &m_equipmentItems },
		{ ItemType::Equipment_ring_2, &m_equipmentItems },
		{ ItemType::Equipment_weapon, &m_equipmentItems },
	});

	// tabbar
	int nTabs = 6;
	float width = nTabs * BUTTON_SIZE.x;
	float height = BUTTON_SIZE.y;

	m_tabBar = new TexturedTabBar();
	m_tabBar->init(sf::FloatRect(0, 0, width, height), nTabs);

	int textureOffset = 0;
	for (int i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_INVENTORYTABS), sf::IntRect(textureOffset, 0, 50, 35));
		textureOffset += 50;
	}

	// init scrolling
	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT, m_window);

	const sf::FloatRect scrollBox(INVENTORY_LEFT + SCROLL_WINDOW_LEFT, GUIConstants::TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	// init empty text
	m_emptyText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_emptyText.setString(g_textProvider->getText("Empty"));

	m_equipment = new InventoryEquipment(getInterface()->getScreen());

	m_currentTab = ItemType::Equipment_weapon;
	setPosition(sf::Vector2f(INVENTORY_LEFT, GUIConstants::TOP));
	reload();
	selectTab(m_currentTab);
}

void Inventory::setPosition(const sf::Vector2f& position) {
	hideDescription();
	deselectCurrentSlot();

	m_position = position;
	m_window->setPosition(position);

	m_selectedTabText.setPosition(position + 
		sf::Vector2f(
			0.5f * INVENTORY_WIDTH - 0.5f * m_selectedTabText.getLocalBounds().width, 
			GUIConstants::TEXT_OFFSET));

	m_goldText.setPosition(position + sf::Vector2f(
		GUIConstants::TEXT_OFFSET, 
		GUIConstants::GUI_WINDOW_HEIGHT - GUIConstants::TEXT_OFFSET - GUIConstants::CHARACTER_SIZE_S));

	sf::Vector2f pos = m_goldText.getPosition();
	pos.x += m_goldText.getBounds().width + 0.2f * GUIConstants::TEXT_OFFSET;
	pos.y += 0.5f * GUIConstants::CHARACTER_SIZE_M - 13.f;
	m_goldSprite.setPosition(pos);

	m_tabBar->setPosition(position + sf::Vector2f(
		0.5f * (INVENTORY_WIDTH - m_tabBar->getTabButtons().size() * BUTTON_SIZE.x), 
		GUIConstants::GUI_TABS_TOP));

	m_scrollWindow.setPosition(position + sf::Vector2f(SCROLL_WINDOW_LEFT, SCROLL_WINDOW_TOP));
	m_scrollBar->setPosition(position + sf::Vector2f(SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, SCROLL_WINDOW_TOP));
	m_scrollHelper->setPosition(position + sf::Vector2f(SCROLL_WINDOW_LEFT, SCROLL_WINDOW_TOP));

	const sf::FloatRect bounds = m_emptyText.getBounds();
	m_emptyText.setPosition(position + sf::Vector2f(
		SCROLL_WINDOW_LEFT + 0.5f * (SCROLL_WINDOW_WIDTH - bounds.width),
		SCROLL_WINDOW_TOP + 0.5f * (SCROLL_WINDOW_WIDTH - bounds.height)));

	m_equipment->setPosition(position - sf::Vector2f(WINDOW_MARGIN + InventoryEquipment::WIDTH, 0.f));

	calculateSlotPositions(*(m_typeMap.at(m_currentTab)));
}

Inventory::~Inventory() {
	delete m_window;
	delete m_descriptionWindow;
	delete m_equipment;
	delete m_currentClone;
	delete m_tabBar;
	delete m_scrollBar;
	delete m_scrollHelper;
	clearAllSlots();
}

void Inventory::clearAllSlots() {
	CLEAR_MAP(m_consumableItems);
	CLEAR_MAP(m_equipmentItems);
	CLEAR_MAP(m_questItems);
	CLEAR_MAP(m_documentItems);
	CLEAR_MAP(m_keyItems);
	CLEAR_MAP(m_miscItems);
	m_selectedSlotId.first = "";
}

InventorySlot* Inventory::getSelectedSlot() const {
	if (m_selectedSlotId.first.empty()) return nullptr;
	if (m_selectedSlotId.second != ItemType::VOID) {
		return m_equipment->getSelectedSlot(m_selectedSlotId.second);
	}
	if (!contains(m_typeMap, m_currentTab)) return nullptr;
	if (!contains(*m_typeMap.at(m_currentTab), m_selectedSlotId.first)) return nullptr;
	return m_typeMap.at(m_currentTab)->at(m_selectedSlotId.first);
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
	if (itemID == "gold") {
		reloadGold();
		return;
	}
	Item* item = g_resourceManager->getItem(itemID);
	if (!item) return;
	if (!contains(m_typeMap, item->getType())) return;

	if (item->isEquipmentType(item->getType())) {
		m_equipment->reload();
	}

	std::map<std::string, InventorySlot*>* tab = m_typeMap.at(item->getType());

	// search for the slot
	if (contains(*tab, item->getID())) {
		if (!contains(m_core->getData().items, itemID)) {
			// the item was removed. check if it is selected.
			if (m_selectedSlotId.first == item->getID()) {
				deselectCurrentSlot();
			}
			tab->erase(item->getID());
			calculateSlotPositions(*(m_typeMap.at(item->getType())));
		}
		else {
			tab->at(item->getID())->setAmount(m_core->getData().items.at(itemID));
		}
		return;
	}

	// the slot for that item has not been found. The slot is added with the current amount in the core
	if (!contains(m_core->getData().items, itemID)) return;
	if (contains(*tab, itemID)) return;
	(*tab).insert({ item->getID(), new InventorySlot(itemID, m_core->getData().items.at(itemID)) });

	calculateSlotPositions(*tab);
}

void Inventory::handleMapRightClick(const InventorySlot* clicked) {
	if (m_mapInterface == nullptr || clicked == nullptr) return;
	if (m_merchantInterface != nullptr) {
		m_merchantInterface->sellItem(clicked->getItem());
		return;
	}
	if (clicked->getItemType() == ItemType::Document)
		showDocument(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Spell)
		learnSpell(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Consumable)
		m_mapInterface->getScreen()->setNegativeTooltip("CannotConsumeItemInMap");
	else if (clicked->getItemType() == ItemType::Permanent)
		dynamic_cast<WorldScreen*>(m_mapInterface->getScreen())->notifyPermanentItemConsumed(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Convertible)
		convertItem(clicked->getItem());
	else if (Item::isEquipmentType(clicked->getItemType()))
		m_equipment->equipItem(clicked);
}

void Inventory::handleLevelRightClick(const InventorySlot* clicked) {
	if (m_levelInterface == nullptr || clicked == nullptr) return;
	if (clicked->getItemType() == ItemType::Consumable)
		m_levelInterface->consumeItem(clicked->getItemID());
	else if (clicked->getItemType() == ItemType::Document)
		showDocument(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Spell)
		learnSpell(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Convertible)
		convertItem(clicked->getItem());
	else if (clicked->getItemType() == ItemType::Permanent)
		dynamic_cast<WorldScreen*>(m_levelInterface->getScreen())->notifyPermanentItemConsumed(clicked->getItem());
	else if (Item::isEquipmentType(clicked->getItemType())) {
		if (m_levelInterface->isBossLevel()) {
			m_levelInterface->getScreen()->setNegativeTooltip("CannotEquipInLevel");
		}
		else {
			m_equipment->equipItem(clicked);
		}
	}
}

void Inventory::updateWindowSelected() {
	m_tabBar->setGamepadEnabled(isWindowSelected());
}

void Inventory::handleMapDoubleClick(const InventorySlot* clicked) const {
	if (m_mapInterface == nullptr || clicked == nullptr) return;

	if (Item::isEquipmentType(clicked->getItemType())) 
		m_equipment->equipItem(clicked);
}

void Inventory::handleLevelDoubleClick(const InventorySlot* clicked) const {
	if (m_levelInterface == nullptr || clicked == nullptr) return;

	if (Item::isEquipmentType(clicked->getItemType())) {
		if (m_levelInterface->isBossLevel()) {
			m_levelInterface->getScreen()->setNegativeTooltip("CannotEquipInLevel");
		}
		else {
			m_equipment->equipItem(clicked);
		}
	}
	else if (clicked->getItemType() == ItemType::Consumable)
		m_levelInterface->equipConsumable(clicked->getItemID());
}

void Inventory::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_scrollBar->update(frameTime);

	// check whether an item was selected
	for (auto& it : *(m_typeMap.at(m_currentTab))) {
		sf::Vector2f pos = it.second->getPosition();
		if (pos.y < GUIConstants::TOP + SCROLL_WINDOW_TOP ||
			pos.y + InventorySlot::SIZE > GUIConstants::TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) continue;
		it.second->update(frameTime);
		if (it.second->isMousedOver() && !m_hasDraggingStarted) {
			selectSlot(it.second->getItemID(), ItemType::VOID);
			if (it.second->isDoubleClicked()) {
				handleLevelDoubleClick(it.second);
				handleMapDoubleClick(it.second);
				break;
			}
			if (it.second->isRightClicked()) {
				handleLevelRightClick(it.second);
				handleMapRightClick(it.second);
				break;
			}
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
		type = ItemType::Key;
	}
	else {
		type = ItemType::Misc;
	}

	if (m_tabBar->getTabButton(activeIndex)->isClicked() && m_currentTab != type) {
		selectTab(type);
	}

	calculateSlotPositions(*(m_typeMap.at(type)));

	// update equipment part
	m_equipment->update(frameTime);
	if (!m_hasDraggingStarted) {
		InventorySlot* eqSlot = m_equipment->getSelectedSlot();
		if (eqSlot != nullptr) {
			selectSlot(eqSlot->getItemID(), eqSlot->getItemType());
		}
	}

	handleDragAndDrop();

	m_window->update(frameTime);
}

void Inventory::selectSlot(const std::string& selectedSlotId, ItemType type) {
	if (selectedSlotId.empty()) {
		deselectCurrentSlot();
		return;
	}

	if (g_inputController->isMouseJustPressedLeftRaw()) {
		m_hasDraggingStarted = true;
		m_isEquipmentSlotDragged = type != ItemType::VOID;
		m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	}

	if (selectedSlotId == m_selectedSlotId.first) return;

	deselectCurrentSlot();
	m_selectedSlotId.first = selectedSlotId;
	m_selectedSlotId.second = type;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot != nullptr) {
		selectedSlot->select();
		showDescription(selectedSlot->getItem(), selectedSlot->isEquipmentOrigin());
	}
}

void Inventory::removeEquipmentItem() {
	if (fastIntersect(*m_window->getBoundingBox(), *m_currentClone->getBoundingBox())) {
		const InventorySlot* is = dynamic_cast<const InventorySlot*>(m_currentClone->getOriginalSlot());
		m_core->equipItem("", is->getItemType());
		reload();
	}
}

void Inventory::handleMapDrag() {
	if (m_mapInterface == nullptr) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot == nullptr) return;
	if (!m_isEquipmentSlotDragged && Item::isEquipmentType(selectedSlot->getItemType())) {
		m_equipment->highlightEquipmentSlot(selectedSlot->getItemType(), true);
	}
	else if (selectedSlot->getItemType() == ItemType::Consumable) {
		m_mapInterface->highlightQuickslots(true);
	}
}

void Inventory::handleLevelDrag() {
	if (m_levelInterface == nullptr) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot == nullptr) return;
	if (selectedSlot->getItemType() == ItemType::Consumable) {
		m_levelInterface->highlightQuickslots(true);
	}
	if (!m_isEquipmentSlotDragged && Item::isEquipmentType(selectedSlot->getItemType()) && !m_levelInterface->isBossLevel()) {
		m_equipment->highlightEquipmentSlot(selectedSlot->getItemType(), true);
	}
}

void Inventory::handleMapDrop() {
	if (m_mapInterface == nullptr || m_currentClone == nullptr) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot == nullptr) return;
	if (m_isEquipmentSlotDragged) {
		removeEquipmentItem();
		return;
	}

	ItemType type = selectedSlot->getItemType();
	if (type == ItemType::Consumable) {
		m_mapInterface->notifyConsumableDrop(m_currentClone);
		m_mapInterface->highlightQuickslots(false);
	}
	else if (Item::isEquipmentType(type)) {
		const InventorySlot* is = dynamic_cast<const InventorySlot*>(m_currentClone->getOriginalSlot());
		m_equipment->highlightEquipmentSlot(is->getItemType(), false);
		m_equipment->notifyEquipmentDrop(m_currentClone);
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
	else if (m_isEquipmentSlotDragged || Item::isEquipmentType(type)) {
		if (m_levelInterface->isBossLevel()) {
			m_levelInterface->getScreen()->setNegativeTooltip("CannotEquipInLevel");
		}
		else {
			m_equipment->notifyEquipmentDrop(m_currentClone);
		}
	}
}

void Inventory::stopDragging() {
	InventorySlot* selectedSlot = getSelectedSlot();
	if (selectedSlot != nullptr) {
		selectedSlot->activate();
		handleLevelDrop();
		handleMapDrop();
	}
	delete m_currentClone;
	m_currentClone = nullptr;
	m_hasDraggingStarted = false;
	m_isDragging = false;
}

void Inventory::handleDragAndDrop() {
	if (!m_hasDraggingStarted) return;
	InventorySlot* selectedSlot = getSelectedSlot();
	if (!(g_inputController->isMousePressedLeft())) {
		stopDragging();
		return;
	}
	sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
	if (!m_isDragging) {
		if (selectedSlot == nullptr) return;
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
	target.draw(m_goldSprite);
	target.draw(m_selectedTabText);

	for (auto& it : *(m_typeMap.at(m_currentTab))) {
		it.second->render(m_scrollHelper->texture);
	}
	m_scrollHelper->render(target);

	m_tabBar->render(target);

	m_descriptionWindow->render(target);

	m_equipment->render(target);
	if (m_currentClone != nullptr) {
		m_currentClone->render(target);
	}

	target.draw(m_scrollWindow);
	m_scrollBar->render(target);

	if (m_typeMap[m_currentTab]->empty()) {
		target.draw(m_emptyText);
	}
}

void Inventory::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	for (auto& it : *(m_typeMap.at(m_currentTab))) {
		it.second->renderAfterForeground(target);
	}
	m_equipment->renderAfterForeground(target);

	m_tabBar->renderAfterForeground(target);
}

void Inventory::convertItem(const Item* item) {
	if (item == nullptr || !item->getCheck().isConvertible) return;

	WorldScreen* worldScreen = getInterface()->getScreen();

	auto const beans = item->getBeans<ItemConvertibleBean>();
	worldScreen->notifyItemChange(item->getID(), -1);

	for (auto const bean : beans) {
		if (bean->probability == 0) continue;
		if (bean->probability == 100) {
			worldScreen->notifyItemChange(bean->convertible_item_id, bean->convertible_amount);
			continue;
		}

		float prob = bean->probability / 100.f;
		int amount = 0;
		for (int i = 0; i < bean->convertible_amount; ++i) {
			if (prob <= randomFloat(0.f, 1.f)) continue;
			++amount;
		}

		worldScreen->notifyItemChange(bean->convertible_item_id, amount);
	}
}

void Inventory::learnSpell(const Item* item) {
	if (item == nullptr || !item->getCheck().isSpell) return;

	WorldScreen* worldScreen = getInterface()->getScreen();

	auto const bean = item->getBean<ItemSpellBean>();
	worldScreen->notifyItemChange(item->getID(), -1);
	worldScreen->notifySpellLearned(static_cast<SpellID>(bean->spell_id));
}

void Inventory::showDocument(const Item* item) {
	if (item == nullptr || !item->getCheck().isDocument) return;

	WorldScreen* worldScreen = getInterface()->getScreen();
	worldScreen->setBook(*item, true);

	// handle possible quest related conditions on this document
	for (auto const bean : item->getBeans<ItemDocumentQuestBean>()) {
		if (bean->quest_desc > 0) {
			worldScreen->notifyQuestDescriptionAdded(bean->quest_name, bean->quest_desc);
		}
		if (!bean->quest_state.empty()) {
			worldScreen->notifyQuestStateChanged(bean->quest_name, resolveQuestState(bean->quest_state));
		}
	}
}

void Inventory::showDescription(const Item* item, bool isEquipmentOrigin) const {
	if (item == nullptr) return;
	m_descriptionWindow->load(*item, m_core, m_merchantInterface ? 
		m_merchantInterface->getMerchantData().receiver_multiplier : 1.f, !isEquipmentOrigin && m_merchantInterface != nullptr);
	m_descriptionWindow->show();
	auto const pos = sf::Vector2f(
		m_window->getPosition().x + WINDOW_MARGIN + m_window->getSize().x,
		m_window->getPosition().y);
	m_descriptionWindow->setPosition(pos);
}

void Inventory::hideDescription() const {
	m_descriptionWindow->hide();
}

void Inventory::selectTab(ItemType type) {
	hideDescription();
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
	case ItemType::Key:
		m_selectedTabText.setString(g_textProvider->getText("Keys"));
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

	m_scrollBar->setScrollPosition(0.f);

	if (m_mapInterface != nullptr) {
		m_mapInterface->showQuickslotBar(type == ItemType::Consumable);
	}
}

void Inventory::reloadGold() {
	std::string gold = "";
	gold.append(g_textProvider->getText("Gold"));
	gold.append(": ");
	gold.append(std::to_string(m_core->getData().gold));
	gold.append("\n\n");
	m_goldText.setString(gold);

	sf::Vector2f pos = m_goldText.getPosition();
	pos.x += m_goldText.getBounds().width + 0.2f * GUIConstants::TEXT_OFFSET;
	pos.y += 0.5f * GUIConstants::CHARACTER_SIZE_M - 13.f;
	m_goldSprite.setPosition(pos);
}

void Inventory::reload() {
	m_scrollBar->scroll(0);
	// reload gold
	reloadGold();

	// reload items
	clearAllSlots();
	hideDescription();
	for (auto& itemData : m_core->getData().items) {
		Item* item = g_resourceManager->getItem(itemData.first);
		if (item == nullptr || !contains(m_typeMap, item->getType())) continue;
		if (contains(*m_typeMap.at(item->getType()), item->getID())) continue;
		m_typeMap.at(item->getType())->insert({ item->getID(), new InventorySlot(item->getID(), itemData.second) });
	}

	// reload equipment
	m_equipment->reload();

	calculateSlotPositions(*(m_typeMap.at(m_currentTab)));
}

void Inventory::calculateSlotPositions(std::map<std::string, InventorySlot*>& slots) {
	float number = static_cast<float>(slots.size());
	int rows = static_cast<int>(std::ceil(number / SLOT_COUNT_X));
	int steps = rows - SLOT_COUNT_Y + 1;

	m_scrollBar->setDiscreteSteps(steps);

	int scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (scrollPos * (ICON_MARGIN + InventorySlot::SIZE) != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = scrollPos * (ICON_MARGIN + InventorySlot::SIZE);
	}

	float animationTime = 0.1f;
	float time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	float start = m_scrollHelper->lastOffset;
	float change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	float effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	float xOffsetStart = m_position.x + SCROLL_WINDOW_LEFT + InventorySlot::ICON_OFFSET + 2 * WINDOW_MARGIN;

	float yOffset = m_position.y + SCROLL_WINDOW_TOP + InventorySlot::ICON_OFFSET + 2 * WINDOW_MARGIN - effectiveScrollOffset;
	float xOffset = xOffsetStart;
	int y = 1;
	int x = 1;
	for (auto& it : slots) {
		it.second->setPosition(sf::Vector2f(xOffset, yOffset));
		if (x + 1 > SLOT_COUNT_X) {
			x = 1;
			xOffset = xOffsetStart;
			y++;
			yOffset += ICON_MARGIN + InventorySlot::SIZE;
		}
		else {
			x++;
			xOffset += ICON_MARGIN + InventorySlot::SIZE;
		}
	}
}

void Inventory::show() {
	m_isVisible = true;
	m_equipment->show();
	if (m_mapInterface != nullptr) {
		m_mapInterface->showQuickslotBar(m_currentTab == ItemType::Consumable);
	}
}

void Inventory::hide() {
	if (m_merchantInterface != nullptr) {
		m_merchantInterface->completeTrade();
		m_merchantInterface = nullptr;
	}
	if (m_mapInterface != nullptr) {
		m_mapInterface->showQuickslotBar(false);
	}
	m_isVisible = false;
	m_equipment->hide();
	stopDragging();
}

void Inventory::startTrading(MerchantInterface* _interface) {
	m_merchantInterface = _interface;
	setPosition(sf::Vector2f(GUIConstants::LEFT_BAR + WINDOW_MARGIN + InventoryEquipment::WIDTH, GUIConstants::TOP));
	show();
}

void Inventory::stopTrading() {
	m_merchantInterface = nullptr;
	setPosition(sf::Vector2f(INVENTORY_LEFT, GUIConstants::TOP));
	hide();
}

InventoryEquipment* Inventory::getEquipment() const {
	return m_equipment;
}
