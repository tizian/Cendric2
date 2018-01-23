#pragma once

#include "global.h"
#include "Slot.h"
#include "GUI/BitmapText.h"
#include "World/Item.h"

class InventorySlot final : public Slot {
public:
	// constructor for filled slots. if amount is < 0, the amount text won't show.
	InventorySlot(const std::string& itemID, int amount, bool isEquipmentOrigin = false);
	// constructor for placeholder slots
	InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos, ItemType equipmentType);

	void setPosition(const sf::Vector2f& pos) override;

	void render(sf::RenderTarget& renderTarget) override;

	void setAmount(int amount);

	const std::string& getItemID() const;
	const Item* getItem() const;
	bool isEquipmentOrigin() const { return m_isEquipmentOrigin; }

	inline ItemType getItemType() const { return m_type; }
	inline void setItemType(ItemType type) { m_type = type; }

	inline float getConfiguredSize() const override { return SIZE; }
	inline float getConfiguredIconOffset() const override { return ICON_OFFSET; }
	

	void setAlpha(sf::Uint8 alpha);

	static const float SIZE;
	static const float ICON_OFFSET;

protected:
	std::string m_itemID;
	ItemType m_type = ItemType::VOID;
	bool m_isEquipmentOrigin = false;

	BitmapText m_amountText;
};