#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "Item.h"

class InventorySlot : public GameObject {
public:
	// constructor for filled slots. if amount is < 0, the amount text won't show.
	InventorySlot(const Item& item, int amount);
	// constructor for placeholder slots
	InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos);
	// default constructor for empty slots
	InventorySlot();

	void select();
	void deselect();
	// activate restores the colors of this slot
	void activate();
	// deactivate sets the colors of this slot all grey.
	void deactivate();

	void highlight(bool highlight);
	void setAmount(int amount);

	void render(sf::RenderTarget& renderTarget) override;
	void onLeftJustPressed() override;
	void onRightClick() override;

	void setPosition(const sf::Vector2f& pos) override;
	void setItemType(ItemType type);

	// asks if it is clicked and sets the member bool to false again (!)
	bool isClicked();
	bool isRightClicked();

	GameObjectType getConfiguredType() const override;
	const Item& getItem() const;
	ItemType getItemType() const;
	const std::string& getItemID() const;

	static const float SIDE_LENGTH;
	static const float MARGIN;

private:
	bool m_isClicked = false;
	bool m_isSelected = false;
	bool m_isRightClicked = false;

	Item m_item;
	ItemType m_type;

	sf::RectangleShape m_inside;
	sf::RectangleShape m_outside;

	BitmapText m_amountText;
};