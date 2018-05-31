#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "Button.h"

class ButtonInterface;
class SelectableWindow;

class ButtonGroup final : public GameObject {
public:
	ButtonGroup(int width = 1);
	~ButtonGroup();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void addButton(ButtonInterface* button);
	void selectButton(int index);

	void setEnabled(bool enabled);
	void setGamepadEnabled(bool enabled);

	void setNextButtonSelectedX(bool forward);
	void setNextButtonSelectedY(bool forward);

	ButtonInterface* getSelectedButton() const;
	ButtonInterface* getButton(int i) const;
	const std::vector<ButtonInterface*>& getButtons() const;
	void clearButtons(bool deleteButtons = true);

	void setSelectableWindow(SelectableWindow* window);

	GameObjectType getConfiguredType() const override;

private:
	std::vector<ButtonInterface*> m_buttons;

	bool m_isEnabled;
	bool m_isGamepadEnabled;
	int m_width;
	int m_selectedButtonIndex;

	void updateSelection();
	int getNextEnabledButtonX(bool forward);
	int getNextEnabledButtonY(bool forward);

	int getSelectedIndexX() const;
	int getSelectedIndexY() const;

	SelectableWindow* m_selectableWindow = nullptr;
};