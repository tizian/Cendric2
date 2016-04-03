#include "ScreenOverlays/WindowScreenOverlay.h"
#include "GUI/GUIConstants.h"

// text offset as seen from the window border.
const sf::Vector2f TEXT_OFFSET = sf::Vector2f(30.f, 15.f);

WindowScreenOverlay::WindowScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : ScreenOverlay(activeTime, fadeTime) {
	
	m_window = new Window(sf::FloatRect(), WindowOrnamentStyle::FANCY, sf::Color(0, 0, 0, 70), sf::Color(0, 0, 0, 70), COLOR_WHITE);
}

WindowScreenOverlay::~WindowScreenOverlay() {
	delete m_window;
}

void WindowScreenOverlay::update(const sf::Time& frameTime) {
	ScreenOverlay::update(frameTime);
	// TODO: update window color
}

void WindowScreenOverlay::render(sf::RenderTarget& renderTarget) {
	m_window->render(renderTarget);
	ScreenOverlay::render(renderTarget);
}

void WindowScreenOverlay::repositionText() {
	const sf::FloatRect& titleBounds = m_title.getLocalBounds();
	m_title.setPosition(0.5f * (WINDOW_WIDTH - titleBounds.width), GUIConstants::TOP + TEXT_OFFSET.y);
	const sf::FloatRect& subtitleBounds = m_subtitle.getLocalBounds();
	m_subtitle.setPosition(0.5f * (WINDOW_WIDTH - subtitleBounds.width), m_title.getPosition().y + titleBounds.height + TEXT_OFFSET.y);

	m_window->setSize(sf::Vector2f(
		2 * TEXT_OFFSET.x + std::max(titleBounds.width, subtitleBounds.width), 
		3 * TEXT_OFFSET.y + titleBounds.height + subtitleBounds.height));

	m_window->setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - m_window->getSize().x), GUIConstants::TOP));
}