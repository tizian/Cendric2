#include "GUI/GUITabBar.h"

#include "GUI/WorldInterface.h"
#include "GUI/GUIConstants.h"
#include "GlobalResource.h"

const int GUITabBar::BUTTON_MARGIN = 24;
const int GUITabBar::WIDTH = 80;

GUITabBar::GUITabBar(WorldInterface* _interface, int size) {
	m_worldInterface = _interface;
	m_size = std::max(0, size);
	init();
}

GUITabBar::~GUITabBar() {
	delete m_window;
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		delete m_buttons[i];
	}
}

void GUITabBar::init() {
	float height = m_size * GUITabButton::SIZE + (m_size - 1) * BUTTON_MARGIN + 4 * GUIConstants::TEXT_OFFSET;
	
	sf::FloatRect box(0, 0, static_cast<float>(WIDTH), height);
	m_window = new Window(box, GUIOrnamentStyle::LARGE, GUIConstants::MAIN_COLOR, GUIConstants::ORNAMENT_COLOR);
	
	for (int i = 0; i < m_size; ++i) {
		GUITabButton* button = new GUITabButton();
		m_buttons.push_back(button);
	}

	setPosition(sf::Vector2f(0.f, 0.f));
}

void GUITabBar::setPosition(const sf::Vector2f& pos) {
	m_window->setPosition(pos);

	sf::Vector2f buttonPos;
	buttonPos.x = pos.x + 0.5f * (WIDTH - GUITabButton::SIZE);
	buttonPos.y = pos.y + 2 * GUIConstants::TEXT_OFFSET;
	for (int i = 0; i < m_size; ++i) {
		m_buttons[i]->setPosition(buttonPos);
		buttonPos.y += GUITabButton::SIZE + BUTTON_MARGIN;
	}
}

void GUITabBar::setButtonTexture(int index, const sf::Texture* tex, int x) {
	if (index < 0 || index > static_cast<int>(m_buttons.size()) - 1) return;

	m_buttons[index]->setTexture(tex, x);
}

void GUITabBar::setButtonText(int index, const std::string& text) {
	if (index < 0 || index > static_cast<int>(m_buttons.size()) - 1) return;

	m_buttons[index]->setText(text);
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

int GUITabBar::getActiveElement() const {
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		if (m_buttons[i]->isActive()) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

void GUITabBar::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	for (size_t i = 0; i < m_buttons.size(); ++i) {
		m_buttons[i]->render(target);
	}
}

void GUITabBar::show(int index) {
	if (index < 0 || index > static_cast<int>(m_buttons.size()) - 1) return;

	for (size_t i = 0; i < m_buttons.size(); ++i) {
		m_buttons[i]->setActive(false);
	}
	
	m_buttons[index]->setActive(true);	

	show();
}

void GUITabBar::show() {
	m_isVisible = true;
}

void GUITabBar::hide() {
	m_isVisible = false;

	for (size_t i = 0; i < m_buttons.size(); ++i) {
		m_buttons[i]->setActive(false);
	}
}

bool GUITabBar::isVisible() const {
	return m_isVisible;
}

WorldInterface* GUITabBar::getInterface() const {
	return m_worldInterface;
}