#include "GUI/InventorySlot.h"

#include "GUI/Inventory.h"
#include "ResourceManager.h"

using namespace std;

const float InventorySlot::SIZE = 58.f;
const float InventorySlot::ICON_OFFSET = 4.f;

InventorySlot::InventorySlot(const Item& item, int amount) : m_item(item.getID()) {
	m_type = m_item.getType();

	m_iconTexture = g_resourceManager->getTexture(ResourceID::Texture_items);
	m_iconTextureRect = sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, static_cast<int>(ICON_SIZE), static_cast<int>(ICON_SIZE));

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(COLOR_WHITE);
	setAmount(amount);

	m_borderTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_inventory);
	m_borderTextureSelected = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_inventory_selected);

	m_highlightTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_highlight);

	initSlot();
}

InventorySlot::InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos) : m_item() {
	m_iconTexture = tex;
	m_iconTextureRect = sf::IntRect(sf::IntRect(texPos.x, texPos.y, static_cast<int>(ICON_SIZE), static_cast<int>(ICON_SIZE)));

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(COLOR_WHITE);

	m_borderTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_inventory);
	m_borderTextureSelected = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_inventory_selected);

	m_highlightTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_highlight);

	m_isEmpty = true;
	initSlot();
}

void InventorySlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + ICON_SIZE - m_amountText.getLocalBounds().width,
		pos.y + ICON_SIZE - m_amountText.getLocalBounds().height));
}

void InventorySlot::render(sf::RenderTarget& renderTarget) {
	Slot::render(renderTarget);
	renderTarget.draw(m_amountText);
}

void InventorySlot::setAmount(int amount) {
	m_amountText.setString(amount < 0 ? "" : to_string(amount));
	setPosition(getPosition());
}