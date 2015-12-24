#include "GUI/HealthBar.h"

HealthBar::HealthBar(const AttributeData* attributes) {
	m_attributes = attributes;

	// init bar
	m_bar.setFillColor(sf::Color::Red);
	m_bar.setSize(sf::Vector2f(0, BAR_HEIGHT));
	m_bar.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + BAR_OUTLINE_THICKNESS));

	m_barOutline.setOutlineColor(sf::Color(150, 0, 0, 255));
	m_barOutline.setOutlineThickness(BAR_OUTLINE_THICKNESS);
	m_barOutline.setFillColor(sf::Color::Transparent);
	m_barOutline.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + BAR_OUTLINE_THICKNESS));
	m_barOutline.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
}

void HealthBar::render(sf::RenderTarget& target) const {
	target.draw(m_barOutline);
	target.draw(m_bar);
}

void HealthBar::update() {
	m_bar.setSize
		(
		sf::Vector2f(BAR_WIDTH * (static_cast<float>(m_attributes->currentHealthPoints) / m_attributes->maxHealthPoints),
		BAR_HEIGHT)
		);
}