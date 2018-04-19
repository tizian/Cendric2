#pragma once

#include "global.h"
#include "World/GameObject.h"

class Button;

class ButtonGroup final : public GameObject {
public:
	ButtonGroup();
	~ButtonGroup();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void addButton(Button* button);

	void setEnabled(bool value);
	void setHorizontal(bool value);

	GameObjectType getConfiguredType() const override;

private:
	std::vector<Button*> m_buttons;
	bool m_isEnabled;

	int m_selectedButtonIndex;
	void updateSelection();
	void selectButton(int index);
	int getNextEnabledButton(bool forward);

	bool m_isHorizontal;
};