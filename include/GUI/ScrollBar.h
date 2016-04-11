#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/SlicedSprite.h"
#include "GUI/GUIConstants.h"

class ScrollBarKnob : public GameObject {
public:
	ScrollBarKnob();

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	bool isPressed() const;

	GameObjectType getConfiguredType() const override;

private:
	bool m_isPressed = false;

	sf::RectangleShape m_knob;

	static const float WIDTH;
	static const float HEIGHT;
};

// A simple ugly scroll bar 
class ScrollBar : public GameObject {
public:
	ScrollBar();

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	// a scroll bar can only be slided if its enabled. also, its color is less opaque if it is disabled.
	void setEnabled(bool enabled);
	void setVisible(bool value);
	void setScrollPosition(float value);

	bool isEnabled() const;
	bool isVisible() const;
	float getScrollPosition() const;
	GameObjectType getConfiguredType() const override;

protected:
	bool m_isPressed = false;
	bool m_isEnabled = true;
	bool m_isVisible = true;
	float m_scrollPosition = 0.f;

	// drag & drop handling
	void handleDragAndDrop();
	// get the slider position from the current mouse position
	float calculateScrollPosition(float mousePosY) const;

	sf::RectangleShape m_background;
	SlicedSprite m_border;
	ScrollBarKnob m_knob;

	static const sf::Color BACKGROUND_COLOR;
	static const sf::Color FILL_COLOR;

	static const float HEIGHT;
	static const float WIDTH;
};