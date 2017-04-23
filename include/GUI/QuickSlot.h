#pragma once

#include "global.h"
#include "Slot.h"

#include "GUI/BitmapText.h"
#include "Item.h"
#include "Enums/EnumNames.h"

class CharacterCore;
class LevelInterface;
class LevelScreen;

class QuickSlot final : public Slot {
public:
	// an empty slot is initialized with an empty itemID
	QuickSlot(LevelInterface* _interface, const std::string& itemID, Key key);

	void setPosition(const sf::Vector2f& pos) override;

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void setItemID(const std::string& itemID);

	void onLeftClick() override;
	void onRightClick() override;

	// reloads the slot, depending on the character core.
	void reload();

	inline float getConfiguredSize() const override { return SIZE; }
	inline float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	static const float SIZE;
	static const float ICON_OFFSET;

private:
	// consumes the item
	void consume();

	LevelScreen* m_screen = nullptr;
	LevelInterface* m_interface = nullptr;
	CharacterCore* m_core = nullptr;
	std::string m_itemID = "";
	Key m_key = Key::VOID;

	BitmapText m_amountText;
	BitmapText m_keyText;
};