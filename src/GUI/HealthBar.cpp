#include "GUI/HealthBar.h"

#include "ResourceManager.h"

void updateTime(sf::Time& time, const sf::Time& frameTime) {	// NOTE: Should maybe be global function instead of GameObject method...?
	if (time == sf::Time::Zero) return;
	time -= frameTime;
	if (time < sf::Time::Zero) time = sf::Time::Zero;
}

HealthBar::HealthBar(const AttributeData* attributes) {
	m_attributes = attributes;
	m_currentHP = attributes->currentHealthPoints;
	m_displayedHP = m_currentHP;

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
	if (newHP < m_currentHP) {	// Hit!
		m_oldHP = m_displayedHP;	// Restart fading from last position
		m_currentHP = newHP;		// Set new target

		// Highlight textures
		m_hitOverlay.setTexture(m_hitOverlayHighlightTexture);

		// Restart timers
		m_waitTime = sf::seconds(WAIT_TIME);
		m_shrinkTime = sf::seconds(SHRINK_TIME);
		m_highlightTime = sf::seconds(HIGHLIGHT_TIME);

		// Reset hit overlay
		float overlayPos = BAR_LEFT + BAR_WIDTH * (static_cast<float>(newHP) / m_attributes->maxHealthPoints);
		float overlayWidth = BAR_WIDTH * (static_cast<float>(m_oldHP - newHP) / m_attributes->maxHealthPoints);
		m_hitOverlay.setPosition(sf::Vector2f(overlayPos, BAR_TOP));
		m_hitOverlay.setSize(sf::Vector2f(overlayWidth, BAR_HEIGHT));
	}

	if (m_highlightTime > sf::Time::Zero) {
		updateTime(m_highlightTime, frameTime);
	}
	else {
		m_hitOverlay.setTexture(m_hitOverlayTexture);
	}

	if (m_waitTime > sf::Time::Zero) {
		updateTime(m_waitTime, frameTime);
	}
	else if (m_shrinkTime > sf::Time::Zero) {
		updateTime(m_shrinkTime, frameTime);

		float scale = m_shrinkTime.asMilliseconds() / (SHRINK_TIME * 1000);
		m_displayedHP = lerp(scale * scale * scale, newHP, m_oldHP);

		float overlayWidth = BAR_WIDTH * (static_cast<float>(m_displayedHP - newHP) / m_attributes->maxHealthPoints);
		m_hitOverlay.setSize(sf::Vector2f(overlayWidth, BAR_HEIGHT));
	}
	
	// Set underlying normal HP bar
	float barWidth = BAR_WIDTH * (static_cast<float>(m_attributes->currentHealthPoints) / m_attributes->maxHealthPoints);
	m_bar.setSize(sf::Vector2f(barWidth, BAR_HEIGHT));
}