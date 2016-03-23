#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/Button.h"
#include "GUI/GUIConstants.h"
#include "GUI/ArrowButton.h"

// An arrow selector gui element, where different elements can be chosen (one is always active)
class ArrowSelector : public GameObject {
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

	// a slider can only be slided if its enabled. also, its color is less opaque if it is disabled.
	void setEnabled(bool enabled);
	void setVisible(bool value);

	bool isEnabled() const;
	bool isVisible() const;

	int getChosenOptionIndex() const;

	GameObjectType getConfiguredType() const override;

protected:
	
	bool m_isEnabled = true;
	bool m_isVisible = true;
	int m_chosenOptionIndex = -1;

	std::vector<std::string> m_options;

	ArrowButton m_rightArrow;
	ArrowButton m_leftArrow;
	BitmapText m_label;

	Button m_button;
};