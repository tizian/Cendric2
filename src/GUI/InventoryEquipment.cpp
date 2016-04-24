#include "GUI/InventoryEquipment.h"
#include "GUI/SlotClone.h"
#include "GUI/GUIConstants.h"
#include "Screens/WorldScreen.h"
#include "Screens/MapScreen.h"

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

void InventoryEquipment::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	for (auto& it : m_slots) {
		it.second.renderAfterForeground(target);
	}
}

void InventoryEquipment::highlightEquipmentSlot(ItemType type, bool highlight) {
	if (m_slots.find(type) == m_slots.end()) return;
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
	const InventorySlot* is = static_cast<const InventorySlot* >(item->getOriginalSlot());
	if (m_slots.find(is->getItemType()) == m_slots.end()) return;
	if (is->getItemType() == ItemType::Equipment_ring_1 || is->getItemType() == ItemType::Equipment_ring_2) {
		if (item->getBoundingBox()->intersects(*(m_slots.at(ItemType::Equipment_ring_1).getBoundingBox()))) {
			m_screen->notifyItemEquip(is->getItemID(), ItemType::Equipment_ring_1);
		}
		else if (item->getBoundingBox()->intersects(*(m_slots.at(ItemType::Equipment_ring_2).getBoundingBox()))) {
			m_screen->notifyItemEquip(is->getItemID(), ItemType::Equipment_ring_2);
		}
	}
	else if (item->getBoundingBox()->intersects(*(m_slots.at(is->getItemType()).getBoundingBox()))) {
		m_screen->notifyItemEquip(is->getItemID(), is->getItemType());
	}
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
	float xOffset = GUIConstants::LEFT + 0.5f * (WIDTH - InventorySlot::SIZE + 2 * InventorySlot::ICON_OFFSET);
	float yOffset = GUIConstants::TOP + YOFFSET + InventorySlot::ICON_OFFSET;

	for (auto& it : types) {
		if (m_core->getEquippedItem(it).empty()) {
			m_slots.insert({ it, InventorySlot(tex, texPos, it) });
		}
		else {
			m_slots.insert({ it, InventorySlot(m_core->getEquippedItem(it), -1) });
		}
		texPos.x += 50;
		m_slots.at(it).setItemType(it);
		m_slots.at(it).setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += InventorySlot::SIZE + MARGIN;
	}
}

void InventoryEquipment::show() {
	m_isVisible = true;
}

void InventoryEquipment::hide() {
	m_isVisible = false;
}