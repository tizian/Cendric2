#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

enum class ButtonOrnamentStyle 
{
	NONE,
	SMALL,
	MEDIUM,
	LARGE
};

// A not so ugly button to click
class Button : public GameObject
{
public:
	Button(const sf::FloatRect& box, ButtonOrnamentStyle style = ButtonOrnamentStyle::NONE);

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	// position will be set automatically as the center of the button.
	// setting text using the text provider (translated)
	void setText(const std::string& text, const sf::Color& color, int charSize);
	void setText(const std::string& text, int charSize);
	void setText(const std::string& text);
	// setting raw text without text provider (not translated)
	void setTextRaw(const std::string& text, const sf::Color& color, int charSize);
	void setTextRaw(const std::string& text, int charSize);
	void setTextRaw(const std::string& text);
	// a button can only be clicked if its enabled. also, the color is less opaque if it is disabled.
	void setEnabled(bool enabled);
	void setCharacterSize(int size);
	void setTextColor(const sf::Color& color);
	void setBackgroundLayerColor(const sf::Color& color);
	void setMainLayerColor(const sf::Color& color);
	void setOrnamentLayerColor(const sf::Color& color);
	void setMouseOverColor(const sf::Color& color);

	bool isClicked() const;
	bool isEnabled() const;
	GameObjectType getConfiguredType() const override;

protected:
	bool m_isMouseOver = false;
	bool m_isPressed = false;
	bool m_isClicked = false;
	bool m_isEnabled = true;

	sf::Vector2f m_positionDefault;
	sf::Vector2f m_backLayerOffset;
	sf::Vector2f m_textOffset;

	SlicedSprite m_mainLayer;
	SlicedSprite m_backLayer;
	SlicedSprite m_ornamentLayer;

	sf::Color m_backLayerColor = CENDRIC_COLOR_LIGHT_PURPLE;
	sf::Color m_mainLayerColor = CENDRIC_COLOR_BLACK;
	sf::Color m_ornamentLayerColor = CENDRIC_COLOR_WHITE;
	sf::Color m_mouseOverColor = CENDRIC_COLOR_PURPLE;

	BitmapText m_text;
};