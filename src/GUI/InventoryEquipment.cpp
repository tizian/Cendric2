#include "GUI/InventoryEquipment.h"

float MARGIN = 10.f;
float YOFFSET = 28.f;
float InventoryEquipment::WIDTH = 100.f;
float InventoryEquipment::HEIGHT = 7 * (InventorySlot::SIDE_LENGTH + MARGIN) - MARGIN + 2 * YOFFSET;
float InventoryEquipment::TOP = 100.f;
float InventoryEquipment::LEFT = 50.f;

InventoryEquipment::InventoryEquipment(CharacterCore* core)
{
	m_core = core;
	
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		CENDRIC_COLOR_TRANS_BLACK, // main
		sf::Color::Transparent, // back
		CENDRIC_COLOR_LIGHT_PURPLE); // ornament
}

InventoryEquipment::~InventoryEquipment()
{
	m_slots.clear();
	delete m_window;
}

void InventoryEquipment::update(const sf::Time& frameTime)
{
	if (!m_isVisible) return;

	// check whether an item was selected
	for (auto& it : m_slots)
	{
		it.update(frameTime);
	}
}

void InventoryEquipment::render(sf::RenderTarget& target)
{
	if (!m_isVisible) return;

	m_window->render(target);
	for (auto& it : m_slots)
	{
		it.render(target);
		// it.renderAfterForeground(target); // uncomment for debug box
	}
}

InventorySlot* InventoryEquipment::getSelectedSlot()
{
	for (auto& it : m_slots)
	{
		if (it.isClicked())
		{
			return &it;
		}
	}
	return nullptr;
}

void InventoryEquipment::reload()
{
	m_slots.clear();

	const sf::Texture* tex = g_resourceManager->getTexture(ResourceID::Texture_equipmentplaceholders);
	sf::Vector2i texPos(0, 0);
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_weapon) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_weapon)), -1)); texPos.x += 50;
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_head) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_head)), -1)); texPos.x += 50;
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_neck) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_neck)), -1)); texPos.x += 50;
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_body) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_body)), -1)); texPos.x += 50;
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_back) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_back)), -1)); texPos.x += 50;
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_ring_1) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_ring_1)), -1)); texPos.x += 50;
	m_slots.push_back(m_core->getEquippedItem(ItemType::Equipment_ring_2) == nullptr ? InventorySlot(tex, texPos) : InventorySlot(*(m_core->getEquippedItem(ItemType::Equipment_ring_2)), -1)); texPos.x += 50;

	float xOffset = LEFT + ((WIDTH - InventorySlot::SIDE_LENGTH) / 2.f);
	float yOffset = TOP + YOFFSET;
	
	for (auto& it : m_slots)
	{
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += InventorySlot::SIDE_LENGTH + MARGIN;
	}
}

void InventoryEquipment::show()
{
	m_isVisible = true;
	reload();
}

void InventoryEquipment::hide()
{
	m_isVisible = false;
}