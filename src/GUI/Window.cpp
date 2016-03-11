#include "GUI/Window.h"

using namespace std;

Window::Window(const sf::FloatRect& box, WindowOrnamentStyle style) : GameObject() {
	// using default values for constructor.
	setBoundingBox(box);

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), COLOR_BLACK, box.width, box.height);

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), COLOR_LIGHT_PURPLE, box.width, box.height);

	if (style == WindowOrnamentStyle::NONE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), COLOR_WHITE, box.width, box.height);
	}
	else if (style == WindowOrnamentStyle::SIMPLE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_large), COLOR_WHITE, box.width, box.height);
	}
	else if (style == WindowOrnamentStyle::FANCY) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_window_ornament), COLOR_WHITE, box.width, box.height);
	}

	setPosition(sf::Vector2f(box.left, box.top));
}

Window::Window(const sf::FloatRect& box, WindowOrnamentStyle style, const sf::Color& mainColor, const sf::Color& backColor, const sf::Color& ornamentColor) {
	setBoundingBox(box);

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), mainColor, box.width, box.height);

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), backColor, box.width, box.height);

	if (style == WindowOrnamentStyle::NONE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), ornamentColor, box.width, box.height);
	}
	else if (style == WindowOrnamentStyle::SIMPLE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_large), COLOR_WHITE, box.width, box.height);
	}
	else if (style == WindowOrnamentStyle::FANCY) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_window_ornament), COLOR_WHITE, box.width, box.height);
	}

	setPosition(sf::Vector2f(box.left, box.top));
}

void Window::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	m_mainLayer.setPosition(position);
	m_backLayer.setPosition(position);
	m_ornamentLayer.setPosition(position);
}

void Window::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_ornamentLayer);
}

void Window::setHeight(float height) {
	if (height < 0.f) return;
	m_boundingBox.height = height;
	m_backLayer.setSize(getSize());
	m_mainLayer.setSize(getSize());
	m_ornamentLayer.setSize(getSize());
}

void Window::setWidth(float width) {
	if (width < 0.f) return;
	m_boundingBox.width = width;
	m_backLayer.setSize(getSize());
	m_mainLayer.setSize(getSize());
	m_ornamentLayer.setSize(getSize());
}

GameObjectType Window::getConfiguredType() const {
	return GameObjectType::_Window;
}