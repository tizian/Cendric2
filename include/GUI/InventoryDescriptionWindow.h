#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "Item.h"
#include "Weapon.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"

class InventoryDescriptionWindow : public Window
{
public:
	InventoryDescriptionWindow(const Item& item);

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

	static const float WIDTH;

private:
	BitmapText m_titleText;
	BitmapText m_descriptionText;
	BitmapText m_statsText;

	std::wstring getAttributeText(const std::string& name, int value);
};