#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "Button.h"

class ButtonInterface;
class SelectableWindow;

class JoystickButtonGroup final : public GameObject {
public:
	JoystickButtonGroup();
	~JoystickButtonGroup() = default;

	void update(const sf::Time& frameTime) override;

	void addButton(ButtonInterface* button);
	void selectButton(int index);

	void setGamepadEnabled(bool enabled);
	void setSelectableWindow(SelectableWindow* window);
	void notifyJoystickDown();

	const std::vector<ButtonInterface*>& getButtons() const;
	GameObjectType getConfiguredType() const override;

private:
	std::vector<ButtonInterface*> m_buttons;

	bool m_isGamepadEnabled;
	int m_selectedButtonIndex;

	bool m_isJoystickDown;

	void updateSelection();
	
	SelectableWindow* m_selectableWindow = nullptr;
};