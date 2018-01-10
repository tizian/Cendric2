#include "GUI/InventorySlot.h"
#include "GUI/Inventory.h"
#include "GUI/GUIConstants.h"
#include "Enums/EnumNames.h"

#include "ResourceManager.h"
#include "GlobalResource.h"

const float InventorySlot::SIZE = 58.f;
const float InventorySlot::ICON_OFFSET = 4.f;

InventorySlot::InventorySlot(const std::string& itemID, int amount, bool isEquipmentOrigin) {
	m_itemID = itemID;
	m_isEquipmentOrigin = isEquipmentOrigin;

	if (itemID == "gold") {
		m_type = ItemType::Gold;

		m_iconTextureRect = sf::IntRect(0, 0, static_cast<int>(ICON_SIZE), static_cast<int>(ICON_SIZE));

	} else {
		Item* item = g_resourceManager->getItem(itemID);
		if (item == nullptr)
			return;
		m_type = item->getType();

		m_iconTextureRect = sf::IntRect(item->getIconTextureLocation().x, item->getIconTextureLocation().y, static_cast<int>(ICON_SIZE), static_cast<int>(ICON_SIZE));
	}

	m_tooltipWindow.setText(g_textProvider->getText(itemID, "item"));

	m_iconTexture = g_resourceManager->getTexture(GlobalResource::TEX_ITEMS);

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(COLOR_WHITE);
	setAmount(amount);

	m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY);
	m_borderTextureSelected = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY_SELECTED);

	m_highlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_HIGHLIGHT);

	initSlot();
}

InventorySlot::InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos, ItemType equipmentType) {
	m_itemID = "";
	m_iconTexture = tex;
	m_iconTextureRect = sf::IntRect(sf::IntRect(texPos.x, texPos.y, static_cast<int>(ICON_SIZE), static_cast<int>(ICON_SIZE)));

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(COLOR_WHITE);
	m_tooltipWindow.setText(g_textProvider->getText(EnumNames::getItemTypeName(equipmentType)) + " (" 
		+ g_textProvider->getText("Empty") + ")");

	m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY);
	m_borderTextureSelected = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY_SELECTED);

	m_highlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_HIGHLIGHT);

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
	m_amountText.setString(amount < 0 ? "" : std::to_string(amount));
	setPosition(getPosition());
}

void InventorySlot::setAlpha(sf::Uint8 alpha) {
	const sf::Color& ic = m_iconRect.getFillColor();
	m_iconRect.setFillColor(sf::Color(ic.r, ic.g, ic.b, alpha));

	const sf::Color& bc = m_borderRect.getFillColor();
	m_borderRect.setFillColor(sf::Color(bc.r, bc.g, bc.b, alpha));

	const sf::Color& bac = m_backgroundRect.getFillColor();
	m_backgroundRect.setFillColor(sf::Color(bac.r, bac.g, bac.b, alpha));

	const sf::Color& tc = m_amountText.getColor();
	m_amountText.setColor(sf::Color(tc.r, tc.g, tc.b, alpha));
}