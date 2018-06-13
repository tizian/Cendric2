#pragma once

#include "global.h"
#include "Slot.h"

#include "GUI/BitmapText.h"
#include "Enums/EnumNames.h"

class CharacterCore;
class WorldInterface;
class WorldScreen;

class QuickSlot final : public Slot {
public:
	// an empty slot is initialized with an empty itemId
	QuickSlot(WorldInterface* _interface, Key key, const std::string& itemId);

	void setPosition(const sf::Vector2f& pos) override;

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void setItemID(const std::string& itemID);

	void onLeftClick() override;
	void onRightClick() override;

	// reloads the slot, depending on the character core.
	void reload();

	float getConfiguredSize() const override { return SIZE; }
	float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	static const float SIZE;
	static const float ICON_OFFSET;

protected:
	void adjustTooltipOffset() override;

private:
	// consumes the item
	void consume();
	void reloadKey();

	WorldScreen* m_screen = nullptr;
	WorldInterface* m_interface = nullptr;
	CharacterCore* m_core = nullptr;
	std::string m_itemID = "";
	Key m_key;

	BitmapText m_amountText;
	BitmapText m_keyText;
};