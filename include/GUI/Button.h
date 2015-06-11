#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

enum class ButtonState {
	DEFAULT,
	MOUSEOVER,
	PRESSED,
};

// A not so ugly button to click
class Button : public GameObject
{
public:
	Button(const sf::FloatRect& box, const sf::Color& pressedColor, const sf::Color& mousoverColor, const sf::Color& releasedColor);
	Button(const sf::FloatRect& box);

	void load() override;
	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

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

	bool isClicked() const;
	bool isEnabled() const;
	GameObjectType getConfiguredType() const override;

private:
	bool m_isPressed = false;
	bool m_isClicked = false;
	bool m_isEnabled = true;

	ButtonState m_state;

	SlicedSprite m_sprite;
	SlicedSprite m_spritePressed;
	SlicedSprite m_spriteMouseover;
	BitmapText m_text;
};