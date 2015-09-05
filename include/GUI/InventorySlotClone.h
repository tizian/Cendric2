#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "GUI/InventorySlot.h"

// class for the inventory slot clone that spawns when an inventory slot is dragged around.
class InventorySlotClone : public GameObject
{
public:
	InventorySlotClone(const InventorySlot* original);

	void render(sf::RenderTarget& renderTarget) override;

	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;
	const std::string& getItemID() const;
	const ItemType getItemType() const;

private:
	const InventorySlot* m_original;

	sf::RectangleShape m_inside;
	sf::RectangleShape m_outside;
};