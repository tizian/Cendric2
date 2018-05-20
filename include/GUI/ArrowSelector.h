#pragma once

#include "global.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/ButtonInterface.h"
#include "GUI/Button.h"
#include "GUI/GUIConstants.h"
#include "GUI/ArrowButton.h"

// An arrow selector gui element, where different elements can be chosen (one is always active)
class ArrowSelector final : public ButtonInterface {
public:
	ArrowSelector();
	
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void recalculatePosition();

	void addOption(const std::string& optionKey);
	void addOptionRaw(const std::string& option);
	void setOptionIndex(int index);

	void setLabelText(const std::string& labelKey);
	void setLabelTextRaw(const std::string& label);

	void setEnabled(bool enabled) override;
	void click() override;

	int getChosenOptionIndex() const;

protected:
	void updateColor() override;

	int m_chosenOptionIndex = -1;

	std::vector<std::string> m_options;

	ArrowButton m_rightArrow;
	ArrowButton m_leftArrow;
	BitmapText m_label;

	Button m_button;
};