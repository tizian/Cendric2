#pragma once

#include "global.h"
#include "Slot.h"
#include "GUI/BitmapText.h"
#include "World/Item.h"

class InventorySlot final : public virtual Slot {
public:
	InventorySlot(const std::string& itemID, int amount, bool isEquipmentOrigin = false);
	InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos, ItemType equipmentType);

	void render(sf::RenderTarget& renderTarget) override;
	void select() override;
	void notifySelection() override;

	void setAmount(int amount);
	void setPosition(const sf::Vector2f& pos) override;
	void setSelectedByButtonGroup(bool isSelected);

	const std::string& getItemID() const;
	const Item* getItem() const;
	bool isEquipmentOrigin() const { return m_isEquipmentOrigin; }
	bool isSelectedByButtonGroup() const { return m_isSelectedByButtonGroup; }

	ItemType getItemType() const { return m_type; }
	void setItemType(ItemType type) { m_type = type; }
	void hideTooltip() const;

	float getConfiguredSize() const override { return SIZE; }
	float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	void setAlpha(sf::Uint8 alpha);

	static const float SIZE;
	static const float ICON_OFFSET;

protected:
	std::string m_itemID;
	ItemType m_type = ItemType::VOID;
	bool m_isEquipmentOrigin = false;
	bool m_isSelectedByButtonGroup = false;

	BitmapText m_amountText;
};