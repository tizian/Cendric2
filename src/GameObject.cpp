#include "GameObject.h"
#include "ResourceManager.h"

void GameObject::render(sf::RenderTarget &renderTarget) {
	// nop
}

void GameObject::renderAfterForeground(sf::RenderTarget &renderTarget) {
	if (m_isDrawBoundingBox) {
		renderTarget.draw(m_debugBox);
	}
}

void GameObject::setDebugBoundingBox(const sf::Color &debugColor) {
	if (!g_resourceManager->getConfiguration().isDebugRendering) return;

	m_debugBox = sf::RectangleShape(sf::Vector2f(m_boundingBox.width, m_boundingBox.height));
	m_debugBox.setPosition(m_position);
	m_debugBox.setOutlineThickness(1.f);
	m_debugBox.setFillColor(sf::Color::Transparent);
	m_debugBox.setOutlineColor(debugColor);
	m_isDrawBoundingBox = true;
}

void GameObject::update(const sf::Time& frameTime) {
	if (g_inputController->isMouseOver(&m_boundingBox, m_isInputInDefaultView)) {
		onMouseOver();
		// if the inputcontroller has locked actions, skip these methods.
		if (g_inputController->isActionLocked()) return;

		if (g_inputController->isKeyJustPressed(Key::Interact)) {
			onInteractKey();
		}
		else if (g_inputController->isRightClicked(&m_boundingBox, m_isInputInDefaultView)) {
			onRightClick();
		}
		else if (g_inputController->isRightJustPressed(&m_boundingBox, m_isInputInDefaultView)) {
			onRightJustPressed();
		}
		else if (g_inputController->isLeftClicked(&m_boundingBox, m_isInputInDefaultView)) {
			onLeftClick();
		}
		else if (g_inputController->isLeftJustPressed(&m_boundingBox, m_isInputInDefaultView)) {
			onLeftJustPressed();
		}
	}
}

void GameObject::setPosition(const sf::Vector2f &position) {
	m_position = position;
	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;

	if (m_isDrawBoundingBox) {
		m_debugBox.setPosition(position);
	}
}

void GameObject::setPositionX(const float posX) {
	const sf::Vector2f newPosition(posX, m_position.y);
	setPosition(newPosition);
}

void GameObject::setPositionY(const float posY) {
	const sf::Vector2f newPosition(m_position.x, posY);
	setPosition(newPosition);
}

void GameObject::setState(GameObjectState state) {
	m_state = state;
}

void GameObject::setBoundingBox(const sf::FloatRect &rect) {
	m_boundingBox = rect;
}

const sf::FloatRect* GameObject::getBoundingBox() const {
	return &m_boundingBox;
}

void GameObject::onMouseOver() {
	// nop
}

void GameObject::onInteractKey() {
	// nop
}

void GameObject::onRightClick() {
	// nop
}

void GameObject::onLeftClick() {
	// nop
}

void GameObject::onRightJustPressed() {
	// nop
}

void GameObject::onLeftJustPressed() {
	// nop
}

sf::Vector2f GameObject::getCenter() const {
	return sf::Vector2f(
		m_boundingBox.left + (m_boundingBox.width / 2),
		m_boundingBox.top + (m_boundingBox.height / 2));
}

bool GameObject::isViewable() const {
	return m_isViewable;
}

bool GameObject::isUpdatable() const {
	return m_isAlwaysUpdate || m_isViewable;
}

void GameObject::setViewable(bool value) {
	m_isViewable = value;
}

void GameObject::setInputInDefaultView(bool value) {
	m_isInputInDefaultView = value;
}

const sf::Vector2f& GameObject::getPosition() const {
	return m_position;
}

bool GameObject::isDisposed() const {
	return m_isDisposed;
}

void GameObject::setDisposed() {
	m_isDisposed = true;
}

void GameObject::setScreen(Screen* screen) {
	m_screen = screen;
}

Screen* GameObject::getScreen() const {
	return m_screen;
}

GameObjectState GameObject::getGameObjectState() const {
	return m_state;
}

void GameObject::updateTime(sf::Time &time, const sf::Time &frameTime) {
	if (time == sf::Time::Zero) return;
	time -= frameTime;
	if (time < sf::Time::Zero) time = sf::Time::Zero;
}
