#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "Button.h"

class ButtonInterface;
class SelectableWindow;

class SpellButtonGroup final : public GameObject {
public:
	SpellButtonGroup();
	~SpellButtonGroup();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void addButton(ButtonInterface* button, int yIndex);
	void selectButton(int yIndex, int xIndex);
	void notifyButtonSelected(int yIndex, int xIndex);
	void setWindowLock(bool isLocked);

	void setEnabled(bool enabled);
	void setGamepadEnabled(bool enabled);

	ButtonInterface* getSelectedButton() const;
	ButtonInterface* getButton(int yIndex, int xIndex) const;
	int getSelectedButtonIdX() const;
	int getSelectedButtonIdY() const;

	void setSelectableWindow(SelectableWindow* window);

	GameObjectType getConfiguredType() const override;

private:
	std::vector<std::vector<ButtonInterface*>> m_buttons;

	bool m_isEnabled;
	bool m_isGamepadEnabled;
	bool m_isWindowLock;
	int m_selectedButtonIndexX;
	int m_selectedButtonIndexY;

	void updateSelection();
	void setNextButtonSelectedX(bool forward);
	void setNextButtonSelectedY(bool forward);
	int getNextEnabledButtonX(bool forward);
	int getNextEnabledButtonY(bool forward);

	SelectableWindow* m_selectableWindow = nullptr;
};