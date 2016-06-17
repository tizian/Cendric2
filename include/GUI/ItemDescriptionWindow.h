#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "Item.h"
#include "Weapon.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"

class ItemDescriptionWindow : public Window {
public:
	ItemDescriptionWindow();
	virtual ~ItemDescriptionWindow();

	void load(const Item& item);

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

	void show();
	void hide();
	bool isVisible() const;

	static const float WIDTH;
	static const float ICON_OFFSET;

protected:
	virtual std::string getGoldLabelText() const;
	virtual std::string getGoldValueText(const Item& item) const;

private:
	BitmapText m_titleText;
	BitmapText m_descriptionText;
	BitmapText m_statsLabelsText;
	BitmapText m_statsValuesText;
	std::vector<std::pair<sf::Vector2f, sf::RectangleShape*>> m_sprites;
	bool m_isVisible = false;
};