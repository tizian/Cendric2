#include "InputController.h"

InputController* g_inputController;

InputController::InputController()
{
}

InputController::~InputController()
{
}

void InputController::update()
{
	// LEFT
    m_keyActiveMap[Key::Left] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Left));
	// RIGHT
	m_keyActiveMap[Key::Right] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Right));
	// JUMP
	m_keyActiveMap[Key::Jump] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Jump));
}

void InputController::init()
{
	m_keyActiveMap.insert(
		{
			{ Key::Left, false },
			{ Key::Right, false },
			{ Key::Jump, false }
		});
	m_keyMap.insert(
		{
			{ Key::Left, sf::Keyboard::A },
			{ Key::Right, sf::Keyboard::D },
			{ Key::Jump, sf::Keyboard::Space }
		});
}

bool InputController::isKeyActive(const Key key)
{
	return m_keyActiveMap[key];
}


