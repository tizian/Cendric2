#pragma once

#include "global.h"
#include "Controller/BaseController.h"

class MouseController : public virtual BaseController {
public:
	MouseController();
	virtual ~MouseController() {};

	void update(const sf::Time& frameTime) override;

	bool isMouseOver(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isLeftClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isRightClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isLeftPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isRightPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isLeftJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isRightJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;

	bool isMousePressedLeft() const;
	bool isMousePressedRight() const;
	bool isMouseJustPressedLeft() const;
	bool isMouseJustPressedLeftRaw() const;
	bool isMouseJustPressedRight() const;
	bool isMouseClickedLeft() const;
	bool isMouseClickedRight() const;

	bool isMouseWheelScrolledDown() const;
	bool isMouseWheelScrolledUp() const;

	// returns mouse position relative to window & the current view
	const sf::Vector2f& getMousePosition() const;
	// returns the mouse position relative to window & the default view
	const sf::Vector2f& getDefaultViewMousePosition() const;

	// input for mouse wheel
	void setMouseWheelScrollTicks(float deltaTicks);

private:
	void updateMouse();
	bool m_isMousePressedLeft = false;
	bool m_isMousePressedRight = false;
	bool m_isMouseJustPressedLeft = false;
	bool m_isMouseJustPressedRight = false;
	bool m_isMouseClickedLeft = false;
	bool m_isMouseClickedRight = false;

	float m_mouseWheelScrollTicks = 0;

	// is the mouse inside our view? we only count mouse clicks if it is so!
	bool m_isMouseInsideView;

	// the mouse position gets calculated once in every frame. This is the mouse position according to the current view
	sf::Vector2f m_mousePosition;
	// the mouse position according to the default view.
	sf::Vector2f m_defaultViewMousePosition;
};