#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "Item.h"
#include "GUI/GUIConstants.h"

class DocumentDescriptionWindow : public Window
{
public:
	DocumentDescriptionWindow(const Item& item);

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

	static const float WIDTH;

private:
	BitmapText m_text;
};