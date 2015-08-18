#include "InputController.h"
#include "ResourceManager.h"

InputController* g_inputController;

InputController::InputController()
{
	m_mainKeyMap = &(g_resourceManager->getConfiguration().mainKeyMap);
	m_alternativeKeyMap = &(g_resourceManager->getConfiguration().alternativeKeyMap);
}

InputController::~InputController()
{
	m_keyActiveMap.clear();
	m_keyJustPressedMap.clear();
}

void InputController::lockAction()
{
	m_isActionLocked = true;
}

bool InputController::isActionLocked() const
{
	return m_isActionLocked;
}

void InputController::update()
{
	m_isWindowFocused = m_mainWindow->hasFocus();
	m_isActionLocked = false;
	
	// update keys
	for (auto& it : m_keyActiveMap)
	{
		m_keyJustPressedMap[it.first] = !m_keyActiveMap[it.first] && 
			(isKeyPressed(m_mainKeyMap->at(it.first)) || isKeyPressed(m_alternativeKeyMap->at(it.first)));
		m_keyActiveMap[it.first] = isKeyPressed(m_mainKeyMap->at(it.first)) || isKeyPressed(m_alternativeKeyMap->at(it.first));
	}
	
	// update mouse clicks
	m_isMouseClickedLeft = (m_isMousePressedLeft && !sf::Mouse::isButtonPressed(sf::Mouse::Left));
	m_isMouseClickedRight = (m_isMousePressedRight && !sf::Mouse::isButtonPressed(sf::Mouse::Right));
	m_isMouseJustPressedLeft = (!m_isMousePressedLeft && sf::Mouse::isButtonPressed(sf::Mouse::Left));
	m_isMouseJustPressedRight = (!m_isMousePressedRight && sf::Mouse::isButtonPressed(sf::Mouse::Right));
	m_isMousePressedLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	m_isMousePressedRight = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	// update mouse positions
	sf::Vector2f pos(sf::Mouse::getPosition((*m_mainWindow)));
	pos.x = pos.x * (static_cast<float>(WINDOW_WIDTH) / m_windowSize.x);
	pos.y = pos.y * (static_cast<float>(WINDOW_HEIGHT) / m_windowSize.y);
	sf::Vector2f view = sf::Vector2f(
		m_mainWindow->getView().getCenter().x - m_mainWindow->getView().getSize().x / 2,
		m_mainWindow->getView().getCenter().y - m_mainWindow->getView().getSize().y / 2);
	sf::Vector2f defaultview = sf::Vector2f(
		m_mainWindow->getDefaultView().getCenter().x - m_mainWindow->getView().getSize().x / 2,
		m_mainWindow->getDefaultView().getCenter().y - m_mainWindow->getView().getSize().y / 2);
	m_mousePosition = pos + view;
	m_defaultViewMousePosition = pos + defaultview;
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
			{ Key::Quickload, false },
			{ Key::Quicksave, false },
			{ Key::Inventory, false },
			{ Key::CharacterInfo, false },
			{ Key::Journal, false },
			{ Key::Interact, false },
			{ Key::Confirm, false },
			{ Key::Left, false },
			{ Key::Right, false },
			{ Key::Up, false },
			{ Key::Down, false },
			{ Key::Jump, false },
			{ Key::Chop, false },
			{ Key::FirstSpell, false },
			{ Key::SecondSpell, false },
			{ Key::ThirdSpell, false },
			{ Key::FourthSpell, false },
			{ Key::FifthSpell, false }
		});
	m_keyJustPressedMap = m_keyActiveMap;
	

	m_windowSize.x = WINDOW_WIDTH;
	m_windowSize.y = WINDOW_HEIGHT;
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

const sf::Vector2f& InputController::getDefaultViewMousePosition() const
{
	return m_defaultViewMousePosition;
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

void InputController::startReadingText()
{
	m_readText.clear();
	m_isReadText = true;
}

void InputController::stopReadingText()
{
	m_isReadText = false;
}

const std::string& InputController::getReadText() const
{
	return m_readText;
}

void InputController::readUnicode(sf::Uint32 character)
{
	if (!m_isReadText || !m_isWindowFocused 
		|| character == '\t' 
		|| character == '\n'
		|| character == '\r') return;
	if (character == '\b')
	{
		if (!m_readText.empty())
			m_readText.erase(m_readText.size() - 1, 1);
	}
	else if (character < 128)
		m_readText += static_cast<char>(character);
}

bool InputController::isKeyPressed(sf::Keyboard::Key key) const
{
	if (key >= sf::Keyboard::KeyCount || key <= sf::Keyboard::Unknown)
	{
		return false;
	}
	return sf::Keyboard::isKeyPressed(key);
}


