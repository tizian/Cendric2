#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

// An ugly button to click
class Button : public GameObject
{
public:
	Button(const sf::FloatRect& box, const sf::Color& pressedColor, const sf::Color& mousoverColor, const sf::Color& releasedColor);
	Button(const sf::FloatRect& box);

	void load() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	// position will be set automatically as the center of the button.
	void setText(Texts text, const sf::Color& color);
	void setText(Texts text);
	// a button can only be clicked if its enabled. also, the color is less opaque if it is disabled.
	void setEnabled(bool enabled);

	bool isPressed() const;
	bool isEnabled() const;
	GameObjectType getConfiguredType() const override;

private:
	bool m_isPressed = false;
	bool m_isEnabled = true;

	sf::Color m_pressedColor;
	sf::Color m_mousoverColor;
	sf::Color m_releasedColor;

	sf::RectangleShape m_shape;
	sf::Text m_text;
};