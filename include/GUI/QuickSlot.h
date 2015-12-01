#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "Item.h"
#include "Enums/EnumNames.h"

class CharacterCore;
class LevelInterface;

class QuickSlot : public GameObject {
public:
	// an empty slot is initialized with an empty itemID
	QuickSlot(LevelInterface* _interface, const std::string& itemID, Key key);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void onLeftJustPressed() override;
	void onRightJustPressed() override;
	void onLeftClick() override;
	void onRightClick() override;
	// reloads the slot, depending on the character core.
	void reload();

	void highlight(bool highlight);

	void setPosition(const sf::Vector2f& pos) override;
	void setItemID(const std::string& itemID);

	GameObjectType getConfiguredType() const override;

	static const float SIDE_LENGTH;
	static const float MARGIN;

private:
	CharacterCore* m_core = nullptr;
	LevelInterface* m_interface = nullptr;
	std::string m_itemID = "";
	bool m_isEmpty = true;
	Key m_key = Key::VOID;

	sf::RectangleShape m_inside;
	sf::RectangleShape m_outside;

	BitmapText m_amountText;
	BitmapText m_keyText;

	// consumes the item
	void consume();
};