#include "GUI/Window.h"
#include "GlobalResource.h"

using namespace std;

Window::Window(const sf::FloatRect& box, GUIOrnamentStyle style) : GameObject() {
	// using default values for constructor.
	m_backColor = COLOR_BLACK;
	m_ornamentColor = COLOR_WHITE;

	init(box, style);
}

Window::Window(const sf::FloatRect& box, GUIOrnamentStyle style, const sf::Color& backColor, const sf::Color& ornamentColor) : GameObject() {
	m_backColor = backColor;
	m_ornamentColor = ornamentColor;

	init(box, style);
}

void Window::init(const sf::FloatRect& box, GUIOrnamentStyle style) {
	setBoundingBox(box);
	m_isInputInDefaultView = true;

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ROUNDED_RECTANGLE), m_backColor, box.width, box.height);
	m_ornamentLayer = SlicedSprite(getOrnamentStyleTexture(style), m_ornamentColor, box.width, box.height);

	setPosition(sf::Vector2f(box.left, box.top));
}

Window::~Window() {
	delete m_closeButton;
}

void Window::addCloseButton(const std::function<void()>& agent) {
	m_closeButton = new Button(sf::FloatRect(0.f, 0.f, 33.f, 33.f), GUIOrnamentStyle::NONE);
	m_closeButton->setOrnamentLayerTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_WINDOW_CLOSE_BUTTON));
	m_closeButton->setOnClick(agent);
	updateCloseButton();
}

void Window::updateCloseButton() {
	if (!m_closeButton) return;
	
	sf::Vector2f pos = getPosition();
	sf::Vector2f size = getSize();

	m_closeButton->setPosition(sf::Vector2f(pos.x + size.x - 33.f, pos.y));

	sf::Color mainColor = m_closeButton->getMainLayerColor();
	mainColor.a = m_ornamentColor.a;
	sf::Color backColor = m_closeButton->getBackgroundLayerColor();
	mainColor.a = m_ornamentColor.a;
	sf::Color mouseOverColor = m_closeButton->getMouseOverColor();
	mouseOverColor.a = m_ornamentColor.a;

	m_closeButton->setOrnamentLayerColor(m_ornamentColor);
	m_closeButton->setMainLayerColor(mainColor);
	m_closeButton->setBackgroundLayerColor(backColor);
	m_closeButton->setMouseOverColor(mouseOverColor);
}

void Window::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	m_backLayer.setPosition(position);
	m_ornamentLayer.setPosition(position);

	updateCloseButton();
}

void Window::update(const sf::Time& frameTime) {
	if (m_closeButton) {
		m_closeButton->update(frameTime);
	}
	GameObject::update(frameTime);
}

void Window::onLeftClick() {
	g_inputController->lockAction();
}

void Window::onRightClick() {
	g_inputController->lockAction();
}

void Window::onLeftJustPressed() {
	g_inputController->lockAction();
}

void Window::onRightJustPressed() {
	g_inputController->lockAction();
}

void Window::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_ornamentLayer);
	if (m_closeButton) {
		m_closeButton->render(renderTarget);
	}
}

void Window::setSize(const sf::Vector2f& size) {
	GameObject::setSize(size);
	setWidth(size.x);
	setHeight(size.y);

	updateCloseButton();
}

void Window::setHeight(float height) {
	if (height < 0.f) return;
	m_boundingBox.height = height;
	m_backLayer.setSize(getSize());
	m_ornamentLayer.setSize(getSize());

	updateCloseButton();
}

void Window::setWidth(float width) {
	if (width < 0.f) return;
	m_boundingBox.width = width;
	m_backLayer.setSize(getSize());
	m_ornamentLayer.setSize(getSize());

	updateCloseButton();
}

void Window::setBackColor(const sf::Color& color) {
	m_backColor = color;
	m_backLayer.setColor(color);

	updateCloseButton();
}

void Window::setOrnamentColor(const sf::Color& color) {
	m_ornamentColor = color;
	m_ornamentLayer.setColor(color);

	updateCloseButton();
}

GameObjectType Window::getConfiguredType() const {
	return _Window;
}