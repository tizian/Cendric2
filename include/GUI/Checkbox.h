#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

// A simple ugly checkbox 
class Checkbox : public GameObject {
public:
	Checkbox();

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	// the text position will be set automatically to the right side of the checkbox.
	// setting text using the text provider (translated)
	void setText(const std::string& text, const sf::Color& color, int charSize);
	void setText(const std::string& text, int charSize);
	void setText(const std::string& text);
	// setting raw text without text provider (not translated)
	void setTextRaw(const std::string& text, const sf::Color& color, int charSize);
	void setTextRaw(const std::string& text, int charSize);
	void setTextRaw(const std::string& text);

	void setCharacterSize(int size);
	void setTextColor(const sf::Color& color);

	// a checkbox can only be clicked if its enabled. also, the margin color is less opaque if it is disabled.
	void setEnabled(bool enabled);
	void setVisible(bool value);
	void setChecked(bool checked);
	void setSymbolColor(const sf::Color& color);
	void setBackgroundColor(const sf::Color& color);
	void setMarginColor(const sf::Color& color);
	void setMouseOverColor(const sf::Color& color);
	
	bool isEnabled() const;
	bool isVisible() const;
	bool isChecked() const;
	GameObjectType getConfiguredType() const override;

protected:
	bool m_isMouseOver = false;
	bool m_isPressed = false;
	bool m_isEnabled = true;
	bool m_isVisible = true;
	bool m_isChecked = false;

	void check();
	void uncheck();

	SlicedSprite m_background;
	SlicedSprite m_margin;
	sf::RectangleShape m_checkedSymbol;
	sf::Vector2f m_textOffset;

	sf::Color m_backgroundColor = COLOR_BLACK;
	sf::Color m_marginColor = COLOR_WHITE;
	sf::Color m_symbolColor = COLOR_WHITE;
	sf::Color m_mouseOverColor = COLOR_PURPLE;
	
	BitmapText m_text;

	static const float SIDE_LENGTH;
	static const float MARGIN;
};