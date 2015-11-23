#include "GUI/InventoryEquipment.h"
#include "GUI/InventorySlotClone.h"

float MARGIN = 10.f;
float YOFFSET = 28.f;
float InventoryEquipment::WIDTH = 100.f;
float InventoryEquipment::HEIGHT = 7 * (InventorySlot::SIDE_LENGTH + MARGIN) - MARGIN + 2 * YOFFSET;
float InventoryEquipment::TOP = 100.f;
float InventoryEquipment::LEFT = 50.f;

InventoryEquipment::InventoryEquipment(CharacterCore* core, bool isInLevel) {
	m_core = core;
	m_isInLevel = isInLevel;

	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		CENDRIC_COLOR_TRANS_BLACK, // main
		sf::Color::Transparent, // back
		CENDRIC_COLOR_LIGHT_PURPLE); // ornament
}

InventoryEquipment::~InventoryEquipment() {
	m_slots.clear();
	delete m_window;
}

void InventoryEquipment::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	// check whether an item was selected
	for (auto& it : m_slots) {
		it.second.update(frameTime);
		if (!m_isInLevel && it.second.isRightClicked()) {
			// unequip item
			m_core->equipItem("", it.first);
			m_requiresReload = true;
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

void InventoryEquipment::highlightEquipmentSlot(ItemType type, bool highlight) {
	if (m_slots.find(type) == m_slots.end()) return;
	if (type == ItemType::Equipment_ring_1 || type == ItemType::Equipment_ring_2) {
		m_slots.at(ItemType::Equipment_ring_1).highlight(highlight);
		m_slots.at(ItemType::Equipment_ring_2).highlight(highlight);
	}
	else {
		m_slots.at(type).highlight(highlight);
	}
}

bool InventoryEquipment::notifyEquipmentDrop(const InventorySlotClone* item) {
	if (item == nullptr || m_slots.find(item->getItemType()) == m_slots.end()) return false;
	if (item->getItemType() == ItemType::Equipment_ring_1 || item->getItemType() == ItemType::Equipment_ring_2) {
		if (item->getBoundingBox()->intersects(*(m_slots.at(ItemType::Equipment_ring_1).getBoundingBox()))) {
			m_core->equipItem(item->getItemID(), ItemType::Equipment_ring_1);
			return true;
		}
		else if (item->getBoundingBox()->intersects(*(m_slots.at(ItemType::Equipment_ring_2).getBoundingBox()))) {
			m_core->equipItem(item->getItemID(), ItemType::Equipment_ring_2);
			return true;
		}
	}
	else if (item->getBoundingBox()->intersects(*(m_slots.at(item->getItemType()).getBoundingBox()))) {
		m_core->equipItem(item->getItemID(), item->getItemType());
		return true;
	}
	return false;
}

InventorySlot* InventoryEquipment::getSelectedSlot() {
	for (auto& it : m_slots) {
		if (it.second.isClicked()) {
			return &it.second;
		}
	}
	return nullptr;
}

InventorySlot* InventoryEquipment::getSelectedSlot(ItemType type) {
	if (m_slots.find(type) == m_slots.end()) return nullptr;
	return &m_slots.at(type);
}

bool InventoryEquipment::requiresReload() {
	bool wasRequireReload = m_requiresReload;
	m_requiresReload = false;
	return wasRequireReload;
}

void InventoryEquipment::reload() {
	m_slots.clear();

	const sf::Texture* tex = g_resourceManager->getTexture(ResourceID::Texture_equipmentplaceholders);

	std::vector<ItemType> types;
	types.push_back(ItemType::Equipment_weapon);
	types.push_back(ItemType::Equipment_head);
	types.push_back(ItemType::Equipment_neck);
	types.push_back(ItemType::Equipment_body);
	types.push_back(ItemType::Equipment_back);
	types.push_back(ItemType::Equipment_ring_1);
	types.push_back(ItemType::Equipment_ring_2);

	sf::Vector2i texPos(0, 0);
	float xOffset = LEFT + ((WIDTH - InventorySlot::SIDE_LENGTH) / 2.f);
	float yOffset = TOP + YOFFSET;

	for (auto& it : types) {
		if (m_core->getEquippedItem(it) == nullptr) {
			m_slots.insert({ it, InventorySlot(tex, texPos) });
		}
		else {
			m_slots.insert({ it, InventorySlot(*(m_core->getEquippedItem(it)), -1) });
		}
		texPos.x += 50;
		m_slots.at(it).setItemType(it);
		m_slots.at(it).setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += InventorySlot::SIDE_LENGTH + MARGIN;
	}
}

void InventoryEquipment::show() {
	m_isVisible = true;
}

void InventoryEquipment::hide() {
	m_isVisible = false;
}