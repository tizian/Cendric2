#pragma once

#include "global.h"
#include "World/GameObject.h"

///<summary>
/// The button interface is used by the ButtonGroup to hold and handle buttons
///</summary>
class ButtonInterface : public virtual GameObject {
public:
	virtual ~ButtonInterface() {}

	virtual void click() = 0;

	virtual void setEnabled(bool enabled);
	void setVisible(bool value);
	void setSelected(bool selected);

	bool isClicked() const;
	bool isPressed() const;
	bool isEnabled() const;
	bool isVisible() const;
	bool isSelected() const;
	bool isVisibleAndEnabled() const;

	GameObjectType getConfiguredType() const override;

protected:
	virtual void updateColor() = 0;

	bool m_isMouseOver = false;
	bool m_isPressed = false;
	bool m_isClicked = false;
	bool m_isEnabled = true;
	bool m_isVisible = true;
	bool m_isSelected = false;
};