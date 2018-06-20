#pragma once

#include "global.h"
#include "Controller/BaseController.h"
#include "Enums/GamepadProductID.h"
#include "Enums/GamepadInput.h"

class GamepadController : public virtual BaseController {
public:
	GamepadController();
	virtual ~GamepadController() {};

	void update(const sf::Time& frameTime);
	
	void notifyGamepadConnected();

	bool isGamepadConnected() const;
	static GamepadProductID getCurrentGamepadProductId();

	bool isLeftJoystickUp() const;
	bool isLeftJoystickDown() const;
	bool isLeftJoystickLeft() const;
	bool isLeftJoystickRight() const;
	bool isLeftJoystickJustUp() const;
	bool isLeftJoystickJustDown() const;
	bool isLeftJoystickJustLeft() const;
	bool isLeftJoystickJustRight() const;

	sf::Vector2f getAnyMoveJoystickAxis() const;
	sf::Vector2f getAimJoystickAxis() const;
	sf::Vector2f getLeftJoystickAxis() const;
	sf::Vector2f getDPadAxis() const;
	GamepadInput getGamepadInputForKey(Key key) const;
	
	void setLastPressedGamepadAxis(sf::Event::JoystickMoveEvent event);
	void setLastPressedGamepadButton(sf::Event::JoystickButtonEvent event);

	// returns the GamepadInput that was pressed in the last frame. If none, returns GamepadInput::VOID
	GamepadInput getLastPressedGamepadInput() const;

	static const int AXIS_THRESHOLD;

protected:
	bool isGamepadButtonPressed(Key key) const;

private:
	const std::map<Key, GamepadInput>* m_joystickMap;

	int m_connectedJoystick;
	GamepadInput m_lastPressedInput = GamepadInput::VOID;

	static std::pair<sf::Joystick::Axis, bool> getAxisForInput(GamepadInput input);
	bool isGamepadInputPressed(GamepadInput input) const;
	bool isAxisNegative(sf::Joystick::Axis axis) const;
	bool isAxisPositive(sf::Joystick::Axis axis) const;
	bool isButtonPressed(int button) const;
	sf::Vector2f getAxisPosition(Key keyRight, Key keyDown) const;

	void updateLeftJoystick();
	bool m_isLeftJoystickLeftPressed = false;
	bool m_isLeftJoystickLeftJustPressed = false;
	bool m_isLeftJoystickRightPressed = false;
	bool m_isLeftJoystickRightJustPressed = false;
	bool m_isLeftJoystickUpPressed = false;
	bool m_isLeftJoystickUpJustPressed = false;
	bool m_isLeftJoystickDownPressed = false;
	bool m_isLeftJoystickDownJustPressed = false;
};
