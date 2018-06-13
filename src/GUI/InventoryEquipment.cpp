#include "GUI/InventoryEquipment.h"
#include "GUI/SlotClone.h"
#include "GUI/GUIConstants.h"
#include "Screens/WorldScreen.h"
#include "Screens/LevelScreen.h"
#include "GlobalResource.h"

const float MARGIN = 10.f;
const float YOFFSET = 0.5f * (GUIConstants::GUI_WINDOW_HEIGHT - 7 * InventorySlot::SIZE - 6 * MARGIN);
const float InventoryEquipment::WIDTH = 100.f;

InventoryEquipment::InventoryEquipment(WorldScreen* screen) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
	const auto lScreen = dynamic_cast<LevelScreen*>(screen);
	m_isModifiable = !lScreen || !lScreen->getWorldData()->isBossLevel;

	// init window
	const sf::FloatRect box(GUIConstants::LEFT, GUIConstants::TOP, WIDTH, GUIConstants::GUI_WINDOW_HEIGHT);
	m_window = new Window(box,
		GUIOrnamentStyle::LARGE,
		COLOR_TRANS_BLACK, // back
		COLOR_WHITE); // ornament

	// order of items displayed
	m_types.push_back(ItemType::Equipment_weapon);
	m_types.push_back(ItemType::Equipment_head);
	m_types.push_back(ItemType::Equipment_neck);
	m_types.push_back(ItemType::Equipment_body);
	m_types.push_back(ItemType::Equipment_back);
	m_types.push_back(ItemType::Equipment_ring_1);
	m_types.push_back(ItemType::Equipment_ring_2);

	setPosition(sf::Vector2f(GUIConstants::LEFT, GUIConstants::TOP));
}

InventoryEquipment::~InventoryEquipment() {
	if (m_buttonGroup) {
		m_buttonGroup->clearButtons(false);
		delete m_buttonGroup;
	}

	CLEAR_MAP(m_slots);
	delete m_window;
}

void InventoryEquipment::setPosition(const sf::Vector2f& position) {
	m_position = position;
	m_window->setPosition(position);

	const auto xOffset = position.x + 0.5f * (WIDTH - InventorySlot::SIZE + 2 * InventorySlot::ICON_OFFSET);
	auto yOffset = position.y + YOFFSET + InventorySlot::ICON_OFFSET;

	for (auto type : m_types) {
		if (!contains(m_slots, type)) continue;
		m_slots.at(type)->setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += InventorySlot::SIZE + MARGIN;
	}
}

void InventoryEquipment::update(const sf::Time& frameTime) const {
	if (!m_isVisible) return;

	m_buttonGroup->update(frameTime);

	// check for mouseover
	auto const& buttons = m_buttonGroup->getButtons();
	for (auto i = 0; i < static_cast<int>(buttons.size()); ++i) {
		if (buttons[i]->isMousedOver()) {
			m_buttonGroup->selectButton(i);
			break;
		}
	}

	// check whether an item was de-equipped
	if (m_isModifiable) {
		updateButtonActions();
	}
}

void InventoryEquipment::updateButtonActions() const {
	const auto selectedButton = dynamic_cast<InventorySlot*>(m_buttonGroup->getSelectedButton());
	if (!selectedButton) return;

	if (selectedButton->isRightClicked() || selectedButton->isDoubleClicked()) {
		m_screen->notifyItemUnequip(selectedButton->getItemID(), selectedButton->getItemType());
	}

	if (!g_inputController->isGamepadConnected() || !isWindowSelected()) {
		return;
	}

	if (g_inputController->isKeyJustPressed(Key::Interact)) {
		m_screen->notifyItemUnequip(selectedButton->getItemID(), selectedButton->getItemType());
	}
}

void InventoryEquipment::render(sf::RenderTarget& target) const {
	if (!m_isVisible) return;

	m_window->render(target);
	m_buttonGroup->render(target);
}

void InventoryEquipment::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	for (auto& it : m_slots) {
		it.second->renderAfterForeground(target);
	}
}

void InventoryEquipment::highlightEquipmentSlot(ItemType type, bool highlight) {
	if (!contains(m_slots, type)) return;
	if (Item::isRingType(type)) {
		if (highlight) {
			m_slots.at(ItemType::Equipment_ring_1)->highlight();
			m_slots.at(ItemType::Equipment_ring_2)->highlight();
		}
		else {
			m_slots.at(ItemType::Equipment_ring_1)->unhighlight();
			m_slots.at(ItemType::Equipment_ring_2)->unhighlight();
		}
	}
	else {
		if (highlight) {
			m_slots.at(type)->highlight();
		}
		else {
			m_slots.at(type)->unhighlight();
		}
	}
}

void InventoryEquipment::notifyEquipmentDrop(const SlotClone* item) {
	if (item == nullptr) return;
	auto const slot = dynamic_cast<const InventorySlot*>(item->getOriginalSlot());
	if (!contains(m_slots, slot->getItemType())) return;
	const auto isRing = Item::isRingType(slot->getItemType());

	if (isRing && fastIntersect(*item->getBoundingBox(), *(m_slots.at(ItemType::Equipment_ring_1)->getBoundingBox()))) {
		m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_1);
	}
	else if (isRing && fastIntersect(*item->getBoundingBox(), *(m_slots.at(ItemType::Equipment_ring_2)->getBoundingBox()))) {
		m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_2);
	}
	else if (!isRing && fastIntersect(*item->getBoundingBox(), *(m_slots.at(slot->getItemType())->getBoundingBox()))) {
		m_screen->notifyItemEquip(slot->getItemID(), slot->getItemType());
	}
	else if (slot->isEquipmentOrigin()) {
		m_screen->notifyItemUnequip(slot->getItemID(), slot->getItemType());
	}
}
void InventoryEquipment::equipRing(const InventorySlot* slot, int slotId) {
	if (slot == nullptr) return;

	if (!Item::isRingType(slot->getItemType())) return;

	if (slotId <= 1) {
		m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_1);
	}
	else {
		m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_2);
	}
}

void InventoryEquipment::equipItem(const InventorySlot* slot) {
	if (slot == nullptr) return;

	if (!contains(m_slots, slot->getItemType())) return;
	if (Item::isRingType(slot->getItemType())) {
		if (m_slots.at(ItemType::Equipment_ring_1)->isEmpty()) {
			m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_1);
		}
		else if (m_slots.at(ItemType::Equipment_ring_2)->isEmpty()) {
			m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_2);
		}
		else {
			m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_1);
		}
	}
	else {
		m_screen->notifyItemEquip(slot->getItemID(), slot->getItemType());
	}
}

InventorySlot* InventoryEquipment::getSelectedSlot() const {
	const auto selectedButton = dynamic_cast<InventorySlot*>(m_buttonGroup->getSelectedButton());
	if ((selectedButton && selectedButton->isMousedOver()) || isWindowSelected()) {
		return selectedButton;
	}

	return nullptr;
}

InventorySlot* InventoryEquipment::getSelectedSlot(ItemType type) {
	if (!contains(m_slots, type)) return nullptr;
	return m_slots.at(type);
}

void InventoryEquipment::reloadButtonGroup() {
	int previouslySelectedId = 0;

	if (m_buttonGroup) {
		previouslySelectedId = m_buttonGroup->getSelectedButtonId();
		m_buttonGroup->clearButtons(false);
		delete m_buttonGroup;
	}
	
	m_buttonGroup = new ButtonGroup();
	m_buttonGroup->setGamepadEnabled(isWindowSelected());
	m_buttonGroup->setSelectableWindow(this);

	for (auto type : m_types) {
		m_buttonGroup->addButton(m_slots[type]);
	}

	m_buttonGroup->selectButton(previouslySelectedId);
}

void InventoryEquipment::reload() {
	CLEAR_MAP(m_slots);

	const sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_EQUIPMENTPLACEHOLDERS);

	sf::Vector2i texPos(0, 0);

	for (auto& it : m_types) {

		if (m_core->getEquippedItem(it).empty()) {
			const auto newSlot = new InventorySlot(tex, texPos, it);
			m_slots.insert({ it, newSlot });
		}
		else {
			const auto newSlot = new InventorySlot(m_core->getEquippedItem(it), -1, true);
			m_slots.insert({ it, newSlot });
		}
		texPos.x += 50;
		m_slots.at(it)->setItemType(it);
	}

	reloadButtonGroup();

	setPosition(m_position);
}

void InventoryEquipment::updateWindowSelected() {
	m_buttonGroup->setGamepadEnabled(isWindowSelected());

	if (isWindowSelected()) {
		m_buttonGroup->getSelectedButton()->setSelected(true);
	}
}

void InventoryEquipment::show() {
	m_isVisible = true;
}

void InventoryEquipment::hide() {
	m_isVisible = false;
}