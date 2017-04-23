#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/SlicedSprite.h"
#include "GUI/GUIConstants.h"

class Window;

class ScrollBarKnob final : public GameObject {
public:
	ScrollBarKnob();

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	void setTexture(sf::Texture* texture);

	bool isPressed() const;

	GameObjectType getConfiguredType() const override;

public:
	static const float WIDTH;
	static const float HEIGHT;

private:
	bool m_isPressed = false;

	sf::RectangleShape m_knob;
};

// A simple ugly scroll bar 
class ScrollBar : public GameObject {
public:
	ScrollBar(float height, const Window* window = nullptr);

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	
	void setTexture(sf::Texture* texture);
	void setKnobTexture(sf::Texture* texture);

	// a scroll bar can only be slided if its enabled. also, its color is less opaque if it is disabled.
	void setEnabled(bool enabled);
	void setVisible(bool value);
	void setScrollPosition(float value, bool animated = false);

	void setDiscreteSteps(int steps);	// Only effective if greater or equal to 2
	void scroll(int direction);

	bool isEnabled() const;
	bool isVisible() const;
	float getScrollPosition() const;
	int getDiscreteScrollPosition() const;

	sf::Time getScrollTime() const;

	GameObjectType getConfiguredType() const override;

public:
	static const float WIDTH;

protected:
	const Window* m_window;
	float m_height;

	int m_discreteSteps;

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

	sf::Time m_time;
};