#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"
#include "GUI/GUIConstants.h"

class SliderKnob : public GameObject {
public:
	SliderKnob();

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	bool isPressed() const;

	GameObjectType getConfiguredType() const override;

private:
	bool m_isMouseOver = false;
	bool m_isPressed = false;

	sf::CircleShape m_knob;

	static const float RADIUS;
	static const float MARGIN;

	static const sf::Color MARGIN_COLOR;
	static const sf::Color FILL_COLOR;
};

// A simple ugly slider 
class Slider : public GameObject {
public:
	Slider(int minPos, int maxPos);

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	// the text position will be set automatically to the right side of the checkbox.
	// setting text using the text provider (translated)
	void setText(const std::string& text);
	// setting raw text without text provider (not translated)
	void setTextRaw(const std::string& text);

	// a slider can only be slided if its enabled. also, its color is less opaque if it is disabled.
	void setEnabled(bool enabled);
	void setVisible(bool value);
	void setSliderPosition(int value);

	bool isEnabled() const;
	bool isVisible() const;
	int getSliderPosition() const;
	GameObjectType getConfiguredType() const override;

protected:
	bool m_isPressed = false;
	bool m_isEnabled = true;
	bool m_isVisible = true;
	int m_sliderPosition = 0;
	int m_minPosition = 0;
	int m_maxPosition = 100;

	// drag & drop handling
	void handleDragAndDrop();
	// get the slider position from the current mouse position
	int calculateSliderPosition(float mousePosX) const;
	// setting the character size of the title, including repositioning
	void setCharacterSize(int size);

	sf::RectangleShape m_background;
	sf::RectangleShape m_filler;
	sf::RectangleShape m_margin;
	SliderKnob m_knob;
	sf::Vector2f m_textOffset;

	static const sf::Color BACKGROUND_COLOR;
	static const sf::Color MARGIN_COLOR;
	static const sf::Color FILL_COLOR;

	std::string m_title;
	BitmapText m_titleText;
	BitmapText m_maxText;
	BitmapText m_minText;

	static const float HEIGHT;
	static const float WIDTH;
	static const float MARGIN;
};