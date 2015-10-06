#include "GUI/InventorySlotClone.h" 

using namespace std;

InventorySlotClone::InventorySlotClone(const InventorySlot* original) {
	m_original = original;

	setBoundingBox(sf::FloatRect(0.f, 0.f, InventorySlot::SIDE_LENGTH, InventorySlot::SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(InventorySlot::SIDE_LENGTH, InventorySlot::SIDE_LENGTH));
	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_items));
	m_inside.setTextureRect(sf::IntRect(
		original->getItem().getIconTextureLocation().x,
		original->getItem().getIconTextureLocation().y,
		static_cast<int>(InventorySlot::SIDE_LENGTH),
		static_cast<int>(InventorySlot::SIDE_LENGTH)));

	m_outside.setSize(sf::Vector2f(InventorySlot::SIDE_LENGTH, InventorySlot::SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(InventorySlot::MARGIN);
	m_outside.setOutlineColor(sf::Color::Red);
}

void InventorySlotClone::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_inside.setPosition(pos);
	m_outside.setPosition(pos);
}

void InventorySlotClone::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_outside);
	renderTarget.draw(m_inside);
}

GameObjectType InventorySlotClone::getConfiguredType() const {
	return GameObjectType::_Interface;
}

const std::string& InventorySlotClone::getItemID() const {
	return m_original->getItemID();
}

const ItemType InventorySlotClone::getItemType() const {
	return m_original->getItemType();
}
