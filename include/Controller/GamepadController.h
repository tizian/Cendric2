#pragma once

#include "global.h"
#include "Controller/BaseController.h"
#include "Enums/GamepadAxis.h"

class GamepadController : public virtual BaseController {
public:
	GamepadController();
	virtual ~GamepadController() {};

	void update(const sf::Time& frameTime);
	
	void notifyJoystickConnected();

	bool isJoystickConnected() const;
	bool isXboxControllerConnected() const;

	bool isLeftJoystickUp() const;
	bool isLeftJoystickDown() const;
	bool isLeftJoystickLeft() const;
	bool isLeftJoystickRight() const;
	bool isLeftJoystickJustUp() const;
	bool isLeftJoystickJustDown() const;
	bool isLeftJoystickJustLeft() const;
	bool isLeftJoystickJustRight() const;

	sf::Vector2f getRightJoystickAxis() const;
	GamepadAxis getGamepadAxisForKey(Key key) const;
	
	void setLastPressedGamepadAxis(sf::Event::JoystickMoveEvent event);
	void setLastPressedGamepadButton(sf::Event::JoystickButtonEvent event);

	// returns the GamepadAxis that was pressed in the last frame. If none, returns GamepadAxis::VOID
	GamepadAxis getLastPressedAxis() const;

protected:
	bool isJoystickButtonPressed(Key key) const;

private:
	const std::map<Key, GamepadAxis>* m_joystickMap;

	std::map<GamepadAxis, std::function<bool()>> m_axisMap;
	void initAxisMap();

	int m_connectedJoystick;
	bool m_isXBoxController;

	GamepadAxis m_lastPressedAxis = GamepadAxis::VOID;

	bool isJoystickAxisPressed(GamepadAxis axis) const;

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

	bool isDPadUp() const;
	bool isDPadDown() const;
	bool isDPadLeft() const;
	bool isDPadRight() const;
	bool isLeftStickUp() const;
	bool isLeftStickDown() const;
	bool isLeftStickLeft() const;
	bool isLeftStickRight() const;
	bool isRightStickUp() const;
	bool isRightStickDown() const;
	bool isRightStickLeft() const;
	bool isRightStickRight() const;
	bool isLeftTrigger() const;
	bool isRightTrigger() const;
	bool isSquare() const;
	bool isCircle() const;
	bool isTriangle() const;
	bool isX() const;
	bool isY() const;
	bool isA() const;
	bool isB() const;
	bool isRightShoulder() const;
	bool isLeftShoulder() const;
	bool isSelect() const;
	bool isStart() const;
	bool isShare() const;
	bool isLeftStickPush() const;
	bool isRightStickPush() const;
	bool isPSButton() const;
	bool isTouchpad() const;
	bool isOptions()const;

	GamepadAxis getLastPressedGamepadAxisXbox(sf::Joystick::Axis axis, bool isNegative);
	GamepadAxis getLastPressedGamepadAxisDs4(sf::Joystick::Axis axis, bool isNegative);
	GamepadAxis getLastPressedGamepadButtonXbox(int button);
	GamepadAxis getLastPressedGamepadButtonDs4(int button);
};