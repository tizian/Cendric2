#include "InputController.h"
#include "ResourceManager.h"

InputController* g_inputController;

InputController::InputController() {
	m_mainKeyMap = &(g_resourceManager->getConfiguration().mainKeyMap);
	m_alternativeKeyMap = &(g_resourceManager->getConfiguration().alternativeKeyMap);
	init();
}

InputController::~InputController() {
	m_keyActiveMap.clear();
	m_keyJustPressedMap.clear();
}

void InputController::lockAction() {
	m_isActionLocked = true;
}

bool InputController::isActionLocked() const {
	return m_isActionLocked;
}

void InputController::update(const sf::Time& frameTime) {
	bool previousFocus = m_isWindowFocused;
	m_isWindowFocused = m_mainWindow->hasFocus();
	m_isActionLocked = false;
	m_lastPressedKey = sf::Keyboard::Unknown;

	if (previousFocus != m_isWindowFocused) {
		g_resourceManager->notifyVolumeChanged();
	}

	// update keys
	for (auto& it : m_keyActiveMap) {
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

	// update mouse wheel
	m_mouseWheelScrollTicks = 0.f;

	// update mouse positions
	sf::Vector2f pos(sf::Mouse::getPosition((*m_mainWindow)));
	pos.x = pos.x / m_windowScale.x;
	pos.y = pos.y / m_windowScale.y;
	sf::Vector2f view = sf::Vector2f(
		m_renderTexture->getView().getCenter().x - m_renderTexture->getView().getSize().x * 0.5f,
		m_renderTexture->getView().getCenter().y - m_renderTexture->getView().getSize().y * 0.5f);
	sf::Vector2f defaultview = sf::Vector2f(
		m_renderTexture->getDefaultView().getCenter().x - m_renderTexture->getView().getSize().x * 0.5f,
		m_renderTexture->getDefaultView().getCenter().y - m_renderTexture->getView().getSize().y * 0.5f);
	m_mousePosition = pos + view;
	m_defaultViewMousePosition = pos + defaultview;
	m_isMouseInsideView = 
		pos.x >= 0.f && pos.x <= static_cast<float>(WINDOW_WIDTH) && 
		pos.y >= 0.f && pos.y <= static_cast<float>(WINDOW_HEIGHT);

	m_cursor.update(frameTime);
}

void InputController::setWindow(sf::RenderWindow* window, sf::RenderTexture* texture) {
	m_mainWindow = window;
	m_renderTexture = texture;
	m_isWindowFocused = m_mainWindow->hasFocus();
	setCurrentWindowSize(m_mainWindow->getSize().x, m_mainWindow->getSize().y);
}

void InputController::init() {
	for (int i = static_cast<int>(Key::VOID) + 1; i < static_cast<int>(Key::MAX); ++i) {
		m_keyActiveMap.insert({ static_cast<Key>(i), false });
	}

	m_keyJustPressedMap = m_keyActiveMap;

	m_windowSize.x = WINDOW_WIDTH;
	m_windowSize.y = WINDOW_HEIGHT;
}

void InputController::setCurrentWindowSize(int width, int height) {
	m_windowSize.x = width;
	m_windowSize.y = height;

	m_windowScale.x = m_windowSize.x / static_cast<float>(WINDOW_WIDTH);
	m_windowScale.y = m_windowSize.y / static_cast<float>(WINDOW_HEIGHT);
	m_cursor.setScale(m_windowScale);
}

const sf::Vector2f& InputController::getMousePosition() const {
	return m_mousePosition;
}

const sf::Vector2f& InputController::getDefaultViewMousePosition() const {
	return m_defaultViewMousePosition;
}

sf::Keyboard::Key InputController::getLastPressedKey() const {
	return m_lastPressedKey;
}

bool InputController::isMouseOver(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isWindowFocused || !m_isMouseInsideView) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition());
}

bool InputController::isRightClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseClickedRight();
}

bool InputController::isLeftClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseClickedLeft();
}

bool InputController::isRightPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMousePressedRight();
}

bool InputController::isLeftPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMousePressedLeft();
}

bool InputController::isRightJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseJustPressedRight();
}

bool InputController::isLeftJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseJustPressedLeft();
}

bool InputController::isMouseClickedLeft() const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return m_isMouseClickedLeft;
}

bool InputController::isMouseClickedRight() const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return m_isMouseClickedRight;
}

bool InputController::isMouseWheelScrolledDown() const {
	return m_mouseWheelScrollTicks < 0.f;
}

bool InputController::isMouseWheelScrolledUp() const {
	return m_mouseWheelScrollTicks > 0.f;
}

bool InputController::isMousePressedLeft() const {
	return m_isWindowFocused && m_isMousePressedLeft;
}

bool InputController::isMousePressedRight() const {
	return m_isWindowFocused && m_isMousePressedRight;
}

bool InputController::isMouseJustPressedLeft() const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return m_isMouseJustPressedLeft;
}

bool InputController::isMouseJustPressedLeftRaw() const {
	return m_isMouseJustPressedLeft;
}

bool InputController::isMouseJustPressedRight() const {
	if (m_isActionLocked || !m_isWindowFocused || !m_isMouseInsideView) return false;
	return m_isMouseJustPressedRight;
}

bool InputController::isKeyActive(Key key) const {
	return m_isWindowFocused && m_keyActiveMap.at(key);
}

bool InputController::isKeyJustPressed(Key key) const {
	if (m_isActionLocked || !m_isWindowFocused) return false;
	return m_keyJustPressedMap.at(key);
}

void InputController::startReadingText() {
	m_readText.clear();
	m_isReadText = true;
}

void InputController::stopReadingText() {
	m_readText.clear();
	m_isReadText = false;
}

const std::string& InputController::getReadText() const {
	return m_readText;
}

void InputController::cropReadText(int maxLength) {
	m_readText = m_readText.substr(0, maxLength);
}

void InputController::readUnicode(sf::Uint32 character) {
	if (!m_isReadText || !m_isWindowFocused
		|| character == '\t'
		|| character == '\n'
		|| character == '\r') return;
	if (character == '\b') {
		if (!m_readText.empty())
			m_readText.erase(m_readText.size() - 1, 1);
	}
	else if (character < 128)
		m_readText += static_cast<char>(character);
}

void InputController::setLastPressedKey(sf::Keyboard::Key key) {
	m_lastPressedKey = key;
}

void InputController::setMouseWheelScrollTicks(float deltaTicks) {
	if ((m_mouseWheelScrollTicks < 0.f && deltaTicks > 0.f) ||
		(m_mouseWheelScrollTicks > 0.f && deltaTicks < 0.f) ||
		(deltaTicks == 0.f)) {
		return;
	}
	m_mouseWheelScrollTicks = deltaTicks;
}

bool InputController::isKeyPressed(sf::Keyboard::Key key) const {
	if (key >= sf::Keyboard::KeyCount || key <= sf::Keyboard::Unknown) {
		return false;
	}
	return sf::Keyboard::isKeyPressed(key);
}

bool InputController::isSelected() const {
	return isKeyJustPressed(Key::Confirm) || isKeyJustPressed(Key::Jump) || isKeyJustPressed(Key::Interact);
}

bool InputController::isScrolledUp() const {
	return isKeyJustPressed(Key::Up) || isMouseWheelScrolledUp();
}

bool InputController::isScrolledDown() const {
	return isKeyJustPressed(Key::Down) || isMouseWheelScrolledDown();
}

Cursor& InputController::getCursor() {
	return m_cursor;
}

bool InputController::isWindowFocused() const {
	return m_isWindowFocused;
}
