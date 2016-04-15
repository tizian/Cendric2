#pragma once

#include "global.h"
#include "Slot.h"
#include "GUI/BitmapText.h"
#include "Item.h"

class InventorySlot : public Slot {
public:
	// constructor for filled slots. if amount is < 0, the amount text won't show.
	InventorySlot(const std::string& itemID, int amount);
	// constructor for placeholder slots
	InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos, ItemType equipmentType);

	void setPosition(const sf::Vector2f& pos) override;

	void render(sf::RenderTarget& renderTarget) override;

	void setAmount(int amount);

	inline const Item& getItem() const { return m_item; }
	inline const std::string& getItemID() const { return m_item.getID(); }

	inline ItemType getItemType() const { return m_type; }
	inline void setItemType(ItemType type) { m_type = type; }

	inline float getConfiguredSize() const override { return SIZE; }
	inline float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	static const float SIZE;
	static const float ICON_OFFSET;

protected:
	Item m_item;
	ItemType m_type;

	BitmapText m_amountText;
};