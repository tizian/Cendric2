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
	virtual std::string getGoldText(const Item& item) const;
	virtual std::string getReputationText(const Item& item) const;
	bool m_isReputationReached = false;

private:
	BitmapText m_titleText;
	BitmapText m_descriptionText;
	BitmapText m_whiteText;
	BitmapText m_coloredText;
	BitmapText m_reputationText;

	bool m_isVisible = false;
};