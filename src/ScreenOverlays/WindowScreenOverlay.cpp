#include "ScreenOverlays/WindowScreenOverlay.h"
#include "GUI/GUIConstants.h"

// text offset as seen from the window border.
const sf::Vector2f TEXT_OFFSET = sf::Vector2f(30.f, 15.f);

WindowScreenOverlay::WindowScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : ScreenOverlay(activeTime, fadeTime) {
	m_windowMainColor = sf::Color(0, 0, 0, 70);
	m_windowBackColor = sf::Color(0, 0, 0, 70);
	m_windowOrnamentColor = COLOR_WHITE;

	m_window = new Window(sf::FloatRect(), WindowOrnamentStyle::FANCY, m_windowMainColor, m_windowBackColor, m_windowOrnamentColor);

	m_window->addCloseButton(std::bind(&WindowScreenOverlay::close, this));
}

WindowScreenOverlay::~WindowScreenOverlay() {
	delete m_window;
}

void WindowScreenOverlay::update(const sf::Time& frameTime) {
	ScreenOverlay::update(frameTime);
	m_window->setMainColor(sf::Color(
		m_windowMainColor.r,
		m_windowMainColor.g,
		m_windowMainColor.b,
		(sf::Uint8)(m_scale * m_windowMainColor.a)));
	m_window->setBackColor(sf::Color(
		m_windowBackColor.r,
		m_windowBackColor.g,
		m_windowBackColor.b,
		(sf::Uint8)(m_scale * m_windowBackColor.a)));
	m_window->setOrnamentColor(sf::Color(
		m_windowOrnamentColor.r,
		m_windowOrnamentColor.g,
		m_windowOrnamentColor.b,
		(sf::Uint8)(m_scale * m_windowOrnamentColor.a)));
	m_window->update(frameTime);
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

void WindowScreenOverlay::close() {
	setDisposed();
}