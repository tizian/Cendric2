#pragma once

#include "global.h"
#include "Controller/BaseController.h"

class GamepadController : public virtual BaseController {
public:
	GamepadController();
	virtual ~GamepadController() {};

	void update(const sf::Time& frameTime);
	
	void notifyJoystickConnected();

	bool isLeftJoystickUp() const;
	bool isLeftJoystickDown() const;
	bool isLeftJoystickLeft() const;
	bool isLeftJoystickRight() const;
	bool isLeftJoystickJustUp() const;
	bool isLeftJoystickJustDown() const;
	bool isLeftJoystickJustLeft() const;
	bool isLeftJoystickJustRight() const;

protected:
	bool isJoystickButtonPressed(Key key) const;

private:
	const std::map<Key, int>* m_joystickMap;
	int m_connectedJoystick;

	bool isJoystickButtonPressed(int button) const;

	void updateLeftJoystick();
	bool m_isLeftJoystickLeftPressed = false;
	bool m_isLeftJoystickLeftJustPressed = false;
	bool m_isLeftJoystickRightPressed = false;
	bool m_isLeftJoystickRightJustPressed = false;
	bool m_isLeftJoystickUpPressed = false;
	bool m_isLeftJoystickUpJustPressed = false;
	bool m_isLeftJoystickDownPressed = false;
	bool m_isLeftJoystickDownJustPressed = false;

	static const int AXIS_THRESHOLD;
};