#include "GUI/Window.h"

using namespace std;

Window::Window(const sf::FloatRect& box, WindowOrnamentStyle style) : GameObject() {
	// using default values for constructor.
	m_mainColor = COLOR_BLACK;
	m_backColor = COLOR_LIGHT_PURPLE;
	m_ornamentColor = COLOR_WHITE;

	init(box, style);
}

Window::Window(const sf::FloatRect& box, WindowOrnamentStyle style, const sf::Color& mainColor, const sf::Color& backColor, const sf::Color& ornamentColor) {
	m_mainColor = mainColor;
	m_backColor = backColor;
	m_ornamentColor = ornamentColor;

	init(box, style);
}

void Window::init(const sf::FloatRect& box, WindowOrnamentStyle style) {
	setBoundingBox(box);

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), m_mainColor, box.width, box.height);

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), m_backColor, box.width, box.height);

	if (style == WindowOrnamentStyle::NONE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), m_ornamentColor, box.width, box.height);
	}
	else if (style == WindowOrnamentStyle::SIMPLE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_large), m_ornamentColor, box.width, box.height);
	}
	else if (style == WindowOrnamentStyle::FANCY) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_window_ornament), m_ornamentColor, box.width, box.height);
	}

	setPosition(sf::Vector2f(box.left, box.top));
}

Window::~Window() {
	if (m_closeButton) {
		delete m_closeButton;
	}
}

void Window::addCloseButton(const std::function<void()>& agent) {
	m_closeButton = new Button(sf::FloatRect(0.f, 0.f, 33.f, 33.f), ButtonOrnamentStyle::WINDOW_CLOSE);
	m_closeButton->setOnClick(agent);
	updateCloseButton();
}

void Window::updateCloseButton() {
	if (!m_closeButton) return;
	
	sf::Vector2f pos = getPosition();
	sf::Vector2f size = getSize();

	m_closeButton->setPosition(sf::Vector2f(pos.x + size.x - 33.f, pos.y));

	sf::Color mainColor(COLOR_BLACK);
	mainColor.a = m_ornamentColor.a;
	sf::Color mouseOverColor(COLOR_LIGHT_PURPLE);
	mouseOverColor.a = m_ornamentColor.a;

	m_closeButton->setOrnamentLayerColor(m_ornamentColor);
	m_closeButton->setMainLayerColor(mainColor);
	m_closeButton->setBackgroundLayerColor(mainColor);
	m_closeButton->setMouseOverColor(mouseOverColor);
}

void Window::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	m_mainLayer.setPosition(position);
	m_backLayer.setPosition(position);
	m_ornamentLayer.setPosition(position);

	updateCloseButton();
}

void Window::update(const sf::Time& frameTime) {
	if (m_closeButton) {
		m_closeButton->update(frameTime);
	}
}

void Window::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
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
	m_mainLayer.setSize(getSize());
	m_ornamentLayer.setSize(getSize());

	updateCloseButton();
}

void Window::setWidth(float width) {
	if (width < 0.f) return;
	m_boundingBox.width = width;
	m_backLayer.setSize(getSize());
	m_mainLayer.setSize(getSize());
	m_ornamentLayer.setSize(getSize());

	updateCloseButton();
}

void Window::setMainColor(const sf::Color& color) {
	m_mainColor = color;
	m_mainLayer.setColor(color);

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
	return GameObjectType::_Window;
}