#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

// A simple checkbox 
class Checkbox final : public GameObject {
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

	void setOnClick(const std::function<void()>& agent);
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
	SlicedSprite m_ornament;
	sf::RectangleShape m_checkedSymbol;
	sf::Vector2f m_textOffset;

	sf::Color m_backgroundColor;
	sf::Color m_ornamentColor;
	sf::Color m_symbolColor;
	sf::Color m_mouseOverColor;
	
	BitmapText m_text;

	static const float SIDE_LENGTH;
	static const float CENTER_SIZE;

private:
	std::function<void()> m_executeOnClick = NOP;
};