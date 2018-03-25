#include "Controller/MouseController.h"
#include "ResourceManager.h"

MouseController::MouseController() {
}

void MouseController::update(const sf::Time& frameTime) {
	updateMouse();
}

void MouseController::updateMouse() {
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
	pos.x = pos.x / (m_windowScale.x * m_spriteScale.x);
	pos.y = pos.y / (m_windowScale.y * m_spriteScale.y);

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
}

const sf::Vector2f& MouseController::getMousePosition() const {
	return m_mousePosition;
}

const sf::Vector2f& MouseController::getDefaultViewMousePosition() const {
	return m_defaultViewMousePosition;
}

bool MouseController::isMouseOver(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition());
}

bool MouseController::isRightClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseClickedRight();
}

bool MouseController::isLeftClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseClickedLeft();
}

bool MouseController::isRightPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView || !m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMousePressedRight();
}

bool MouseController::isLeftPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView || !m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMousePressedLeft();
}

bool MouseController::isRightJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseJustPressedRight();
}

bool MouseController::isLeftJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return boundingBox->contains(useDefaultView ? getDefaultViewMousePosition() : getMousePosition()) && isMouseJustPressedLeft();
}

bool MouseController::isMouseClickedLeft() const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return m_isMouseClickedLeft;
}

bool MouseController::isMouseClickedRight() const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return m_isMouseClickedRight;
}

bool MouseController::isMouseWheelScrolledDown() const {
	if (!m_isMouseInsideView || !m_isWindowFocused) return false;
	return m_mouseWheelScrollTicks < 0.f;
}

bool MouseController::isMouseWheelScrolledUp() const {
	if (!m_isMouseInsideView || !m_isWindowFocused) return false;
	return m_mouseWheelScrollTicks > 0.f;
}

bool MouseController::isMousePressedLeft() const {
	if (!m_isMouseInsideView || !m_isWindowFocused) return false;
	return m_isMousePressedLeft;
}

bool MouseController::isMousePressedRight() const {
	if (!m_isMouseInsideView || !m_isWindowFocused) return false;
	return m_isMousePressedRight;
}

bool MouseController::isMouseJustPressedLeft() const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return m_isMouseJustPressedLeft;
}

bool MouseController::isMouseJustPressedLeftRaw() const {
	return m_isMouseJustPressedLeft;
}

bool MouseController::isMouseJustPressedRight() const {
	if (!m_isMouseInsideView || m_isActionLocked || !m_isWindowFocused) return false;
	return m_isMouseJustPressedRight;
}

void MouseController::setMouseWheelScrollTicks(float deltaTicks) {
	if ((m_mouseWheelScrollTicks < 0.f && deltaTicks > 0.f) ||
		(m_mouseWheelScrollTicks > 0.f && deltaTicks < 0.f) ||
		(deltaTicks == 0.f)) {
		return;
	}
	m_mouseWheelScrollTicks = deltaTicks;
}