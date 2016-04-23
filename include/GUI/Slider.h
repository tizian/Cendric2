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
	bool m_isPressed = false;

	sf::RectangleShape m_knob;

	static const float WIDTH;
	static const float HEIGHT;
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

	// Optionally set a unit that is displayed at the end of tht title
	void setUnit(const std::string& unit);

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
	SlicedSprite m_border;
	SliderKnob m_knob;
	sf::Vector2f m_textOffset;

	std::string m_title;
	std::string m_unit;
	BitmapText m_titleText;
	BitmapText m_maxText;
	BitmapText m_minText;

	static const float HEIGHT;
	static const float WIDTH;
	static const float BORDER_OFFSET;
};