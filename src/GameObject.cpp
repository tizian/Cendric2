#include "GameObject.h"
#include "ResourceManager.h"
#include "GameObjectComponents/GameObjectComponent.h"

GameObject::~GameObject() {
	for (auto& component : m_components) {
		delete component;
	}
	m_components.clear();
}

void GameObject::render(sf::RenderTarget& renderTarget) {
	// nop
}

void GameObject::renderAfterForeground(sf::RenderTarget& renderTarget) {
	if (m_isDebugRendering) {
		renderTarget.draw(m_debugBox);
	}
	for (auto& component : m_components) {
		component->renderAfterForeground(renderTarget);
	}
}

void GameObject::setDebugBoundingBox(const sf::Color& debugColor) {
	m_debugBox = sf::RectangleShape(getSize());
	m_debugBox.setPosition(getPosition());
	m_debugBox.setOutlineThickness(1.f);
	m_debugBox.setFillColor(COLOR_TRANSPARENT);
	m_debugBox.setOutlineColor(debugColor);
}

void GameObject::update(const sf::Time& frameTime) {
	m_isDebugRendering = g_resourceManager->getConfiguration().isDebugRendering;
	
	for (auto& component : m_components) {
		component->update(frameTime);
	}
	if (g_inputController->isMouseOver(&m_boundingBox, m_isInputInDefaultView)) {
		onMouseOver();
		// if the inputcontroller has locked actions, skip these methods.
		if (g_inputController->isActionLocked()) return;

		if (g_inputController->isRightClicked(&m_boundingBox, m_isInputInDefaultView)) {
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

void GameObject::setPosition(const sf::Vector2f& position) {
	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;

	if (m_isDebugRendering) {
		m_debugBox.setPosition(position);
	}
	for (auto& component : m_components) {
		component->setPosition(position);
	}
}

void GameObject::setPositionX(const float posX) {
	const sf::Vector2f newPosition(posX, m_boundingBox.top);
	setPosition(newPosition);
}

void GameObject::setPositionY(const float posY) {
	const sf::Vector2f newPosition(m_boundingBox.left, posY);
	setPosition(newPosition);
}

void GameObject::setState(GameObjectState state) {
	m_state = state;
}

void GameObject::setBoundingBox(const sf::FloatRect& rect) {
	m_boundingBox.width = rect.width;
	m_boundingBox.height = rect.height;

	if (m_isDebugRendering) {
		m_debugBox.setSize(getSize());
	}

	setPosition(sf::Vector2f(rect.left, rect.top));
}

void GameObject::setSize(const sf::Vector2f& size) {
	setBoundingBox(sf::FloatRect(m_boundingBox.left, m_boundingBox.top, size.x, size.y));
}

sf::Vector2f GameObject::getPosition() const {
	return sf::Vector2f(m_boundingBox.left, m_boundingBox.top);
}

sf::Vector2f GameObject::getSize() const {
	return sf::Vector2f(m_boundingBox.width, m_boundingBox.height);
}

sf::Vector2f GameObject::getCenter() const {
	return sf::Vector2f(
		m_boundingBox.left + (m_boundingBox.width / 2),
		m_boundingBox.top + (m_boundingBox.height / 2));
}

const sf::FloatRect* GameObject::getBoundingBox() const {
	return &m_boundingBox;
}

void GameObject::onMouseOver() {
	for (auto& component : m_components) {
		component->onParentMouseOver();
	}
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

void GameObject::addComponent(GameObjectComponent* component) {
	m_components.push_back(component);
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