#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

#include "Enums/ItemID.h"
#include "Structs/ItemBean.h"
#include "ItemFactory.h"
#include "Item.h"

class LootWindow : public Window
{
public:
	LootWindow(const sf::FloatRect& box, WindowOrnamentStyle style);
	LootWindow(const sf::FloatRect& box, WindowOrnamentStyle style, const sf::Color& mainColor, const sf::Color& backColor, const sf::Color& ornamentColor);
	~LootWindow();

	void render(sf::RenderTarget& renderTarget) override;
	void setLoot(const std::map<ItemID, int>& loot, int gold);
	void setPosition(const sf::Vector2f& position) override;

private:
	BitmapText* m_lootText = nullptr;
	void loadItemNames(const std::map<ItemID, int>& loot, std::map<ItemID, std::wstring>& names) const;
};