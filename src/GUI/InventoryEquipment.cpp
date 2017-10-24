#include "GUI/InventoryEquipment.h"
#include "GUI/SlotClone.h"
#include "GUI/GUIConstants.h"
#include "Screens/WorldScreen.h"
#include "Screens/MapScreen.h"
#include "GlobalResource.h"

float MARGIN = 10.f;
float YOFFSET = 0.5f * (GUIConstants::GUI_WINDOW_HEIGHT - 7 * InventorySlot::SIZE - 6 * MARGIN);
float InventoryEquipment::WIDTH = 100.f;

InventoryEquipment::InventoryEquipment(WorldScreen* screen) {
	m_screen = screen;
	m_core = screen->getCharacterCore();
	m_isInLevel = (dynamic_cast<MapScreen*>(screen) == nullptr);

	// init window
	sf::FloatRect box(GUIConstants::LEFT, GUIConstants::TOP, WIDTH, GUIConstants::GUI_WINDOW_HEIGHT);
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
	m_slots.clear();
	delete m_window;
}

void InventoryEquipment::setPosition(const sf::Vector2f& position) {
	m_position = position;
	m_window->setPosition(position);

	float xOffset = position.x + 0.5f * (WIDTH - InventorySlot::SIZE + 2 * InventorySlot::ICON_OFFSET);
	float yOffset = position.y + YOFFSET + InventorySlot::ICON_OFFSET;

	for (auto type : m_types) {
		if (!contains(m_slots, type)) continue;
		m_slots.at(type).setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += InventorySlot::SIZE + MARGIN;
	}
}

void InventoryEquipment::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	// check whether an item was selected
	for (auto& it : m_slots) {
		it.second.update(frameTime);
		if (!m_isInLevel && (it.second.isRightClicked() || it.second.isDoubleClicked())) {
			// unequip item
			m_screen->notifyItemEquip("", it.first);
			break;
		}
	}
}

void InventoryEquipment::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	for (auto& it : m_slots) {
		it.second.render(target);
	}
}

void InventoryEquipment::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	for (auto& it : m_slots) {
		it.second.renderAfterForeground(target);
	}
}

void InventoryEquipment::highlightEquipmentSlot(ItemType type, bool highlight) {
	if (!contains(m_slots, type)) return;
	if (type == ItemType::Equipment_ring_1 || type == ItemType::Equipment_ring_2) {
		if (highlight) {
			m_slots.at(ItemType::Equipment_ring_1).highlight();
			m_slots.at(ItemType::Equipment_ring_2).highlight();
		}
		else {
			m_slots.at(ItemType::Equipment_ring_1).unhighlight();
			m_slots.at(ItemType::Equipment_ring_2).unhighlight();
		}
	}
	else {
		if (highlight) {
			m_slots.at(type).highlight();
		}
		else {
			m_slots.at(type).unhighlight();
		}
	}
}

void InventoryEquipment::notifyEquipmentDrop(const SlotClone* item) {
	if (item == nullptr) return;
	const InventorySlot* slot = static_cast<const InventorySlot* >(item->getOriginalSlot());
	if (!contains(m_slots, slot->getItemType())) return;
	if (slot->getItemType() == ItemType::Equipment_ring_1 || slot->getItemType() == ItemType::Equipment_ring_2) {
		if (fastIntersect(*item->getBoundingBox(), *(m_slots.at(ItemType::Equipment_ring_1).getBoundingBox()))) {
			m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_1);
		}
		else if (fastIntersect(*item->getBoundingBox(), *(m_slots.at(ItemType::Equipment_ring_2).getBoundingBox()))) {
			m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_2);
		}
	}
	else if (fastIntersect(*item->getBoundingBox(), *(m_slots.at(slot->getItemType()).getBoundingBox()))) {
		m_screen->notifyItemEquip(slot->getItemID(), slot->getItemType());
	}
}

void InventoryEquipment::equipItem(const InventorySlot* slot) {
	if (slot == nullptr) return;

	if (!contains(m_slots, slot->getItemType())) return;
	if (slot->getItemType() == ItemType::Equipment_ring_1 || slot->getItemType() == ItemType::Equipment_ring_2) {
		if (m_slots.at(ItemType::Equipment_ring_1).isEmpty()) {
			m_screen->notifyItemEquip(slot->getItemID(), ItemType::Equipment_ring_1);
		}
		else if (m_slots.at(ItemType::Equipment_ring_2).isEmpty()) {
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

InventorySlot* InventoryEquipment::getSelectedSlot() {
	for (auto& it : m_slots) {
		if (it.second.isMousedOver()) {
			return &it.second;
		}
	}
	return nullptr;
}

InventorySlot* InventoryEquipment::getSelectedSlot(ItemType type) {
	if (!contains(m_slots, type)) return nullptr;
	return &m_slots.at(type);
}

void InventoryEquipment::reload() {
	m_slots.clear();

	const sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_EQUIPMENTPLACEHOLDERS);

	sf::Vector2i texPos(0, 0);

	for (auto& it : m_types) {
		if (m_core->getEquippedItem(it).empty()) {
			m_slots.insert({ it, InventorySlot(tex, texPos, it) });
		}
		else {
			m_slots.insert({ it, InventorySlot(m_core->getEquippedItem(it), -1) });
		}
		texPos.x += 50;
		m_slots.at(it).setItemType(it);
	}

	setPosition(m_position);
}

void InventoryEquipment::show() {
	m_isVisible = true;
}

void InventoryEquipment::hide() {
	m_isVisible = false;
}