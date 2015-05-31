#include "InputController.h"

InputController* g_inputController;

InputController::InputController()
{
}

InputController::~InputController()
{
	m_keyActiveMap.clear();
	m_keyJustPressedMap.clear();
	m_keyJustPressedMap.clear();
}

void InputController::update()
{
	m_isWindowFocused = m_mainWindow->hasFocus();
	// LEFT
	m_keyJustPressedMap[Key::Left] = !m_keyActiveMap[Key::Left] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Left));
    m_keyActiveMap[Key::Left] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Left));
	// RIGHT
	m_keyJustPressedMap[Key::Right] = !m_keyActiveMap[Key::Right] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Right));
	m_keyActiveMap[Key::Right] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Right));
	// UP
	m_keyJustPressedMap[Key::Up] = !m_keyActiveMap[Key::Up] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Up));
	m_keyActiveMap[Key::Up] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Up));
	// DOWN
	m_keyJustPressedMap[Key::Down] = !m_keyActiveMap[Key::Down] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Down));
	m_keyActiveMap[Key::Down] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Down));
	// JUMP
	m_keyJustPressedMap[Key::Jump] = !m_keyActiveMap[Key::Jump] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Jump));
	m_keyActiveMap[Key::Jump] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Jump));
	// Escape
	m_keyJustPressedMap[Key::Escape] = !m_keyActiveMap[Key::Escape] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Escape));
	m_keyActiveMap[Key::Escape] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Escape));
	// Inventory
	m_keyJustPressedMap[Key::Inventory] = !m_keyActiveMap[Key::Inventory] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Inventory));
	m_keyActiveMap[Key::Inventory] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::Inventory));
	// Character Info
	m_keyJustPressedMap[Key::CharacterInfo] = !m_keyActiveMap[Key::CharacterInfo] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::CharacterInfo));
	m_keyActiveMap[Key::CharacterInfo] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::CharacterInfo));

	// SPELL FIRE
	m_keyJustPressedMap[Key::SpellFire] = !m_keyActiveMap[Key::SpellFire] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellFire));
	m_keyActiveMap[Key::SpellFire] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellFire));
	// SPELL ICE
	m_keyJustPressedMap[Key::SpellIce] = !m_keyActiveMap[Key::SpellIce] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellIce));
	m_keyActiveMap[Key::SpellIce] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellIce));
	// SPELL CHOP
	m_keyJustPressedMap[Key::SpellChop] = !m_keyActiveMap[Key::SpellChop] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellChop));
	m_keyActiveMap[Key::SpellChop] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellChop));
	// SPELL FORCEFIELD
	m_keyJustPressedMap[Key::SpellForcefield] = !m_keyActiveMap[Key::SpellForcefield] && sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellForcefield));
	m_keyActiveMap[Key::SpellForcefield] = sf::Keyboard::isKeyPressed(m_keyMap.at(Key::SpellForcefield));

	// update mouse clicks
	m_isMouseClickedLeft = (m_isMousePressedLeft && !sf::Mouse::isButtonPressed(sf::Mouse::Left));
	m_isMouseClickedRight = (m_isMousePressedRight && !sf::Mouse::isButtonPressed(sf::Mouse::Right));
	m_isMouseJustPressedLeft = (!m_isMousePressedLeft && sf::Mouse::isButtonPressed(sf::Mouse::Left));
	m_isMouseJustPressedRight = (!m_isMousePressedRight && sf::Mouse::isButtonPressed(sf::Mouse::Right));
	m_isMousePressedLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	m_isMousePressedRight = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	// update mouse position
	sf::Vector2f pos(sf::Mouse::getPosition((*m_mainWindow)));
	pos.x = pos.x * (static_cast<float>(WINDOW_WIDTH) / m_windowSize.x);
	pos.y = pos.y * (static_cast<float>(WINDOW_HEIGHT + BOTTOM_BORDER) / m_windowSize.y);
	sf::Vector2f view = sf::Vector2f(
		m_mainWindow->getView().getCenter().x - m_mainWindow->getView().getSize().x / 2,
		m_mainWindow->getView().getCenter().y - m_mainWindow->getView().getSize().y / 2);
	m_mousePosition = pos + view;
}

void InputController::setWindow(sf::RenderWindow* window)
{
	m_mainWindow = window;
	m_isWindowFocused = m_mainWindow->hasFocus();
}


void InputController::init()
{
	m_keyActiveMap.insert(
		{
			{ Key::Escape, false },
			{ Key::Inventory, false },
			{ Key::CharacterInfo, false },
			{ Key::Left, false },
			{ Key::Right, false },
			{ Key::Up, false },
			{ Key::Down, false },
			{ Key::Jump, false },
			{ Key::SpellFire, false },
			{ Key::SpellChop, false },
			{ Key::SpellForcefield, false },
			{ Key::SpellIce, false }
		});
	m_keyJustPressedMap.insert(
	{
		{ Key::Escape, false },
		{ Key::Inventory, false },
		{ Key::CharacterInfo, false },
		{ Key::Left, false },
		{ Key::Right, false },
		{ Key::Up, false },
		{ Key::Down, false },
		{ Key::Jump, false },
		{ Key::SpellFire, false },
		{ Key::SpellChop, false },
		{ Key::SpellForcefield, false },
		{ Key::SpellIce, false }
	});
	m_keyMap.insert(
		{
			{ Key::Escape, sf::Keyboard::Escape },
			{ Key::Inventory, sf::Keyboard::I },
			{ Key::CharacterInfo, sf::Keyboard::C },
			{ Key::Left, sf::Keyboard::A },
			{ Key::Right, sf::Keyboard::D },
			{ Key::Up, sf::Keyboard::W },
			{ Key::Down, sf::Keyboard::S },
			{ Key::Jump, sf::Keyboard::Space },
			{ Key::SpellChop, sf::Keyboard::Num1 },
			{ Key::SpellFire, sf::Keyboard::Num2 },
			{ Key::SpellIce, sf::Keyboard::Num3 },
			{ Key::SpellForcefield, sf::Keyboard::Num4 }
		});

	m_windowSize.x = WINDOW_WIDTH;
	m_windowSize.y = WINDOW_HEIGHT + BOTTOM_BORDER;
}

void InputController::setCurrentWindowSize(int width, int height)
{
	m_windowSize.x = width;
	m_windowSize.y = height;
}

const sf::Vector2f& InputController::getMousePosition() const
{
	return m_mousePosition;
}

bool InputController::isMouseOver(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition());
}

bool InputController::isRightClicked(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition()) && isMouseClickedRight();
}

bool InputController::isLeftClicked(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition()) && isMouseClickedLeft();
}

bool InputController::isRightPressed(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition()) && isMousePressedRight();
}

bool InputController::isLeftPressed(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition()) && isMousePressedLeft();
}

bool InputController::isRightJustPressed(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition()) && isMouseJustPressedRight();
}

bool InputController::isLeftJustPressed(const sf::FloatRect* boundingBox) const
{
	return boundingBox->contains(getMousePosition()) && isMouseJustPressedLeft();
}

bool InputController::isMouseClickedLeft() const
{
	return m_isWindowFocused && m_isMouseClickedLeft;
}

bool InputController::isMouseClickedRight() const
{
	return m_isWindowFocused && m_isMouseClickedRight;
}

bool InputController::isMousePressedLeft() const
{
	return m_isWindowFocused && m_isMousePressedLeft;
}

bool InputController::isMousePressedRight() const
{
	return m_isWindowFocused && m_isMousePressedRight;
}

bool InputController::isMouseJustPressedLeft() const
{
	return m_isWindowFocused && m_isMouseJustPressedLeft;
}

bool InputController::isMouseJustPressedRight() const
{
	return m_isWindowFocused && m_isMouseJustPressedRight;
}

bool InputController::isKeyActive(Key key)
{
	return m_isWindowFocused && m_keyActiveMap[key];
}

bool InputController::isKeyJustPressed(Key key)
{
	return m_isWindowFocused && m_keyJustPressedMap[key];
}


