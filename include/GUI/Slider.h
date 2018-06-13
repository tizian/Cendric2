#pragma once

#include "global.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/ButtonInterface.h"
#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"
#include "GUI/GUIConstants.h"

class SliderKnob final : public GameObject {
public:
	SliderKnob();

	void onLeftJustPressed() override;
	void onLeftClick() override;
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

// A simple slider 
class Slider final : public ButtonInterface {
public:
	Slider(int minPos, int maxPos);

	void onLeftJustPressed() override;
	void onLeftClick() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void click() override;
	void setPosition(const sf::Vector2f& pos) override;

	// the text position will be set automatically to the right side of the checkbox.
	// setting text using the text provider (translated)
	void setText(const std::string& text);
	// setting raw text without text provider (not translated)
	void setTextRaw(const std::string& text);

	// Optionally set a unit that is displayed at the end of tht title
	void setUnit(const std::string& unit);

	void setEnabled(bool enabled) override;
	void setSliderPosition(int value);

	int getSliderPosition() const;

protected:
	int m_sliderPosition = 0;
	int m_minPosition = 0;
	int m_maxPosition = 100;

	void updateColor() override;
	void handleDragAndDrop();
	int calculateSliderPosition(float mousePosX) const;
	void setCharacterSize(int size);

	sf::RectangleShape m_background;
	sf::RectangleShape m_filler;
	SlicedSprite m_border;
	SliderKnob m_knob;
	sf::Vector2f m_textOffset;
	sf::Time m_scrollTimeout;

	std::string m_title;
	std::string m_unit;
	BitmapText m_titleText;
	BitmapText m_maxText;
	BitmapText m_minText;

	static const float HEIGHT;
	static const float WIDTH;
	static const float BORDER_OFFSET;
};