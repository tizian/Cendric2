#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"

///
/// A tooltip for each slot. Created by the corresponding slots.
/// Is rendered on mouseover.
///
class TooltipWindow : public Window {
public:
	TooltipWindow();
	
	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;
	void setText(const std::string& text);
	void setTextOffset(const sf::Vector2f& offset);
	void setTextAlignment(TextAlignment alignment);

	static const int MAX_WIDTH;

private:
	BitmapText m_text;
	sf::Vector2f m_textOffset;
};