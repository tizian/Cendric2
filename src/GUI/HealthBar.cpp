#include "GUI/HealthBar.h"

#include "ResourceManager.h"
#include "GameObject.h"

#include <iostream>
using namespace std;

HealthBar::HealthBar(const AttributeData* attributes) {
	m_attributes = attributes;
	m_currentHP = attributes->currentHealthPoints;
	m_overlayHP = m_currentHP;
	m_maxOverlayHP = m_overlayHP;

	m_borderTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_healthbar_border);
	m_barTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_healthbar_content);
	m_hitOverlayTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_healthbar_content_hit);
	m_hitOverlayHighlightTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_healthbar_content_highlight);

	// init bar
	m_bar.setPosition(sf::Vector2f(BAR_LEFT, BAR_TOP));
	m_bar.setSize(sf::Vector2f(0.f, BAR_HEIGHT));
	m_bar.setTexture(m_barTexture);

	// init border
	m_border.setPosition(sf::Vector2f(BAR_LEFT - BORDER_OFFSET_X, BAR_TOP - BORDER_OFFSET_Y));
	m_border.setSize(sf::Vector2f(BAR_WIDTH + 2.f * BORDER_OFFSET_X, BAR_HEIGHT + 2.f * BORDER_OFFSET_Y));
	m_border.setTexture(m_borderTexture);

	// init overlay
	m_hitOverlay.setPosition(sf::Vector2f(BAR_LEFT, BAR_TOP));
	m_hitOverlay.setSize(sf::Vector2f(0.f, 0.f));
	m_hitOverlay.setTexture(m_hitOverlayTexture);

	m_highlightTime = sf::Time::Zero;
	m_waitTime = sf::Time::Zero;
	m_shrinkTime = sf::Time::Zero;
}

void HealthBar::render(sf::RenderTarget& target) const {
	target.draw(m_bar);
	target.draw(m_hitOverlay);
	target.draw(m_border);
}

void HealthBar::update(const sf::Time& frameTime) {
	int newHP = m_attributes->currentHealthPoints;
	bool adjustOverlay = false;
	if (newHP < m_currentHP) {
		// Restart shrinking from last position
		m_maxOverlayHP = m_currentHP;
		m_overlayHP = m_maxOverlayHP;

		// Highlight textures
		m_hitOverlay.setTexture(m_hitOverlayHighlightTexture);

		// Restart timers
		m_waitTime = sf::seconds(WAIT_TIME);
		m_shrinkTime = sf::seconds(SHRINK_TIME);
		m_highlightTime = sf::seconds(HIGHLIGHT_TIME);

		m_currentHP = newHP;
	}
	else if (newHP > m_currentHP) {
		m_currentHP = newHP;
	}

	// Highlight on hit
	if (m_highlightTime > sf::Time::Zero) {
		GameObject::updateTime(m_highlightTime, frameTime);
		if (m_highlightTime <= sf::Time::Zero) {
			m_hitOverlay.setTexture(m_hitOverlayTexture);
		}
	}

	// Shrinking animation on hit
	if (m_waitTime > sf::Time::Zero) {
		// Wait before shrinking
		GameObject::updateTime(m_waitTime, frameTime);
	}
	else if (m_shrinkTime > sf::Time::Zero) {
		// Shrinking.
		GameObject::updateTime(m_shrinkTime, frameTime);

		float scale = m_shrinkTime.asSeconds() / SHRINK_TIME;
		m_overlayHP = lerp(scale * scale * scale, m_currentHP, m_maxOverlayHP);
	}
	else {
		m_overlayHP = m_currentHP;
	}

	// Set normal bar
	float normalWidth = BAR_WIDTH * (static_cast<float>(m_currentHP) / m_attributes->maxHealthPoints);
	m_bar.setSize(sf::Vector2f(normalWidth, BAR_HEIGHT));

	// Set overlay
	float overlayX = BAR_LEFT + normalWidth;
	float overlayWidth = std::max(0.f, BAR_WIDTH * (static_cast<float>(m_overlayHP - m_currentHP) / m_attributes->maxHealthPoints));
	m_hitOverlay.setPosition(sf::Vector2f(overlayX, BAR_TOP));
	m_hitOverlay.setSize(sf::Vector2f(overlayWidth, BAR_HEIGHT));
}