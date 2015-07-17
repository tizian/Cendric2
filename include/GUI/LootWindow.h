#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

#include "Structs/ItemBean.h"
#include "Item.h"

class LootWindow : public Window
{
public:
	LootWindow(const sf::FloatRect& box);
	~LootWindow();

	void render(sf::RenderTarget& renderTarget) override;
	void setLoot(const std::map<std::string, int>& loot, int gold);
	void setPosition(const sf::Vector2f& position) override;

private:
	BitmapText* m_lootText = nullptr;
	void loadItemNames(const std::map<std::string, int>& loot, std::map<std::string, std::wstring>& names) const;
};