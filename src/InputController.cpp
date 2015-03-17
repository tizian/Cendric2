#include "stdafx.h"

InputController::InputController()
{
}

InputController::~InputController()
{
}

void InputController::update()
{
	// LEFT
	m_keyActiveMap[Key::LEFT] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::LEFT));
	// RIGHT
	m_keyActiveMap[Key::RIGHT] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::RIGHT));
	// JUMP
	m_keyActiveMap[Key::JUMP] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::JUMP));
}

void InputController::init()
{
	m_keyActiveMap.insert(
		{
			{ Key::LEFT, false },
			{ Key::RIGHT, false },
			{ Key::JUMP, false }
		});
	m_keyMap.insert(
		{
			{ Key::LEFT, sf::Keyboard::Left },
			{ Key::RIGHT, sf::Keyboard::Right },
			{ Key::JUMP, sf::Keyboard::Space }
		});
}

unordered_map<Key, bool> InputController::getKeyActiveMap() 
{
	return m_keyActiveMap;
}

unordered_map<Key, sf::Keyboard::Key> InputController::getKeyMap()
{
	return m_keyMap;
}


