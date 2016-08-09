#include "GUI/GUITabBar.h"

#include "Level/LevelInterface.h"
#include "Map/MapInterface.h"
#include "GUI/WorldInterface.h"
#include "GUI/GUIConstants.h"
#include "GUI/GUITabButton.h"
#include "GlobalResource.h"

GUITabBar::GUITabBar(LevelInterface* _interface) {
	m_levelInterface = _interface;
	init();
}

GUITabBar::GUITabBar(MapInterface* _interface) {
	m_mapInterface = _interface;
	init();
}

GUITabBar::~GUITabBar() {
	delete m_window;
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		delete m_buttons[i];
	}
}

void GUITabBar::init() {
	int n = 4;
	if (m_mapInterface) {
		n = 5;
	}

	WIDTH = 80.f;
	HEIGHT = n * GUITabButton::SIZE + (n - 1) * 2 * GUIConstants::BUTTON_MARGIN + 4 * GUIConstants::TEXT_OFFSET;
	LEFT = GUIConstants::LEFT_BAR;
	TOP = GUIConstants::TOP + 0.5f * (GUIConstants::GUI_WINDOW_HEIGHT - HEIGHT);

	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box, GUIOrnamentStyle::LARGE, GUIConstants::MAIN_COLOR, GUIConstants::ORNAMENT_COLOR);

	float x = LEFT + 0.5f * (WIDTH - GUITabButton::SIZE);
	float y = TOP + 2 * GUIConstants::TEXT_OFFSET;

	for (int i = 0; i < n; ++i) {
		GUITabButton *button = new GUITabButton(GUIElement(i));
		button->setPosition(sf::Vector2f(x, y));
		m_buttons.push_back(button);

		y += GUITabButton::SIZE + 2 * GUIConstants::BUTTON_MARGIN;
	}
}

void GUITabBar::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	if (!getInterface()->isGuiOverlayVisible()) {
		hide();
	}

	int nextActiveIndex = -1;
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		GUITabButton* tab = m_buttons[i];
		tab->update(frameTime);
		if (tab->isClicked() && !tab->isActive()) {
			nextActiveIndex = static_cast<int>(i);
		}
	}

	if (nextActiveIndex != -1) {
		for (size_t i = 0; i < m_buttons.size(); ++i) {
			m_buttons[i]->setActive(false);
		}
		m_buttons[nextActiveIndex]->setActive(true);
	}
}

void GUITabBar::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		m_buttons[i]->render(target);
	}
}

void GUITabBar::renderAfterForeground(sf::RenderTarget& target) {

}

void GUITabBar::show() {
	m_isVisible = true;
}

void GUITabBar::hide() {
	m_isVisible = false;
}

bool GUITabBar::isVisible() const {
	return m_isVisible;
}

WorldInterface* GUITabBar::getInterface() const {
	if (m_mapInterface == nullptr) {
		return m_levelInterface;
	}
	return m_mapInterface;
}