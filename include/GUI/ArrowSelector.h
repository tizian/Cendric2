#pragma once

#include "global.h"
#include "GameObject.h"
#include "TextProvider.h"
#include "ResourceManager.h"

#include "GUI/Button.h"
#include "GUI/GUIConstants.h"

class ArrowSelectorArrow : public GameObject {
public:
	ArrowSelectorArrow(bool pointRight);

	void onLeftClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setEnabled(bool value);

	bool isClicked() const;
	
	GameObjectType getConfiguredType() const override;

private:
	bool m_isMouseOver = false;
	bool m_wasMouseOver = false;
	bool m_isClicked = false;
	bool m_isEnabled = false;

	sf::Sprite m_arrow;

	static const sf::Color MOUSEOVER_COLOR;
	static const sf::Color MAIN_COLOR;
	static const sf::Color DISABLED_COLOR;
};

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

	ArrowSelectorArrow m_rightArrow;
	ArrowSelectorArrow m_leftArrow;
	BitmapText m_label;

	Button m_button;
};