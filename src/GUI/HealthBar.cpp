#include "GUI/HealthBar.h"
#include "ResourceManager.h"
#include "GUI/GUIConstants.h"
#include "GlobalResource.h"

HealthBar::HealthBar(const AttributeData* attributes, HealthBarStyle style) {
	if (style == HealthBarStyle::MainCharacter) {
		m_barWidth = 300.f;
		m_barLeft = 18.f;
		m_barTop = 18.f;
		
		m_borderOffsetX = 13.f;
		m_borderOffsetY = 10.f;

		m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_MAINCHAR_BORDER);
	}
	else if (style == HealthBarStyle::Enemy) {
		m_barWidth = 200.f;
		m_barLeft = WINDOW_WIDTH - m_barWidth - 2.f * 18.f;
		m_barTop = 18.f;
		
		m_borderOffsetX = 8.f;
		m_borderOffsetY = 6.f;

		m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_ENEMY_BORDER);
	}
	else if (style == HealthBarStyle::Boss) {
		m_barWidth = 300.f;
		m_barLeft = WINDOW_WIDTH - m_barWidth - 2.f * 18.f;
		m_barTop = 18.f;

		m_borderOffsetX = 12.f;
		m_borderOffsetY = 10.f;

		m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_BOSS_BORDER);
	}

	m_barTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT);
	m_hitOverlayTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT_HIT);
	m_hitOverlayHighlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT_HIGHLIGHT);

	// init bar
	m_bar.setPosition(sf::Vector2f(m_barLeft, m_barTop));
	m_bar.setSize(sf::Vector2f(0.f, BAR_HEIGHT));
	m_bar.setTexture(m_barTexture);

	// init border
	m_border.setPosition(sf::Vector2f(m_barLeft - m_borderOffsetX, m_barTop - m_borderOffsetY));
	m_border.setSize(sf::Vector2f(m_barWidth + 2.f * m_borderOffsetX, BAR_HEIGHT + 2.f * m_borderOffsetY));
	m_border.setTexture(m_borderTexture);

	// init overlay
	m_hitOverlay.setPosition(sf::Vector2f(m_barLeft, m_barTop));
	m_hitOverlay.setSize(sf::Vector2f(0.f, 0.f));
	m_hitOverlay.setTexture(m_hitOverlayTexture);

	m_highlightTime = sf::Time::Zero;
	m_waitTime = sf::Time::Zero;
	m_shrinkTime = sf::Time::Zero;

	m_tooltipWindow.setPosition(sf::Vector2f(m_barLeft + 0.5f * m_barWidth, m_barTop + BAR_HEIGHT + 10.f));
	m_tooltipWindow.setTextAlignment(TextAlignment::Center);

	setAttributes(attributes);

	m_name.setTextAlignment(TextAlignment::Center);
	m_name.setTextStyle(TextStyle::Shadowed);
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	setName("");
}

HealthBar::~HealthBar() {
}

const AttributeData* HealthBar::getAttributes() const {
	return m_attributes;
}

void HealthBar::setAttributes(const AttributeData* attributes) {
	m_attributes = attributes;

	if (attributes) {
		m_currentHP = attributes->currentHealthPoints;
	}
	else {
		m_currentHP = 0;
	}
	m_overlayHP = m_currentHP;
	m_maxOverlayHP = m_overlayHP;
}

void HealthBar::setName(const std::string& name) {
	m_name.setString(name);
	sf::FloatRect bounds = m_name.getBounds();
	m_name.setPosition(m_barLeft + 0.5f * (m_barWidth - bounds.width), m_barTop + BAR_HEIGHT + 10.f);
}

void HealthBar::setVisible(bool visible) {
	m_isVisible = visible;
}

void HealthBar::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	target.draw(m_bar);
	target.draw(m_hitOverlay);
	target.draw(m_border);
	target.draw(m_name);
	if (m_showTooltip) {
		m_tooltipWindow.render(target);
	}
}

void HealthBar::update(const sf::Time& frameTime) {
	if (m_attributes == nullptr) return;

	int newHP = m_attributes->currentHealthPoints;
	if (newHP < m_currentHP) {
		m_currentHP = newHP;

		// Restart shrinking from last position
		m_maxOverlayHP = m_overlayHP;

		// Highlight textures
		m_hitOverlay.setTexture(m_hitOverlayHighlightTexture);

		// Restart timers
		m_shrinkTime = sf::seconds(SHRINK_TIME);
		m_highlightTime = sf::seconds(HIGHLIGHT_TIME);
	}
	else if (newHP > m_currentHP) {
		m_currentHP = newHP;
	}

	// Highlight on hit
	if (m_highlightTime > sf::Time::Zero) {
		updateTime(m_highlightTime, frameTime);
		if (m_highlightTime <= sf::Time::Zero) {
			m_hitOverlay.setTexture(m_hitOverlayTexture);
		}
	}

	// Shrinking animation on hit
	if (m_waitTime > sf::Time::Zero && m_shrinkTime > sf::Time::Zero) {
		// Wait before shrinking
		updateTime(m_waitTime, frameTime);
	}
	else if (m_shrinkTime > sf::Time::Zero) {
		// Shrinking.
		updateTime(m_shrinkTime, frameTime);

		float scale = m_shrinkTime.asSeconds() / SHRINK_TIME;
		m_overlayHP = static_cast<int>(lerp(scale * scale * scale, static_cast<float>(m_currentHP), static_cast<float>(m_maxOverlayHP)));
	}
	else {
		m_overlayHP = m_currentHP;
		m_waitTime = sf::seconds(WAIT_TIME);
	}

	// Set normal bar
	float normalWidth = m_barWidth * (static_cast<float>(m_currentHP) / m_attributes->maxHealthPoints);
	m_bar.setSize(sf::Vector2f(normalWidth, BAR_HEIGHT));

	// Set overlay
	float overlayX = m_barLeft + normalWidth;
	float overlayWidth = std::max(0.f, m_barWidth * (static_cast<float>(m_overlayHP - m_currentHP) / m_attributes->maxHealthPoints));
	m_hitOverlay.setPosition(sf::Vector2f(overlayX, m_barTop));
	m_hitOverlay.setSize(sf::Vector2f(overlayWidth, BAR_HEIGHT));

	// Update tooltip
	sf::FloatRect rect(m_barLeft, m_barTop, m_barWidth, BAR_HEIGHT);
	m_showTooltip = g_inputController->isMouseOver(&rect, true);
	m_tooltipWindow.setText(std::to_string(m_currentHP) + "/" + std::to_string(m_attributes->maxHealthPoints));
}