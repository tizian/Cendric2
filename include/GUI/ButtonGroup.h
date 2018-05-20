#pragma once

#include "global.h"
#include "World/GameObject.h"

class ButtonInterface;

class ButtonGroup final : public GameObject {
public:
	ButtonGroup();
	~ButtonGroup();

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void addButton(ButtonInterface* button);

	void setEnabled(bool value);
	void setHorizontal(bool value);

	void setNextButtonSelected(bool forward);
	ButtonInterface* getSelectedButton() const;

	GameObjectType getConfiguredType() const override;

private:
	std::vector<ButtonInterface*> m_buttons;
	bool m_isEnabled;

	int m_selectedButtonIndex;
	void updateSelection();
	void selectButton(int index);
	int getNextEnabledButton(bool forward);

	bool m_isHorizontal;
};