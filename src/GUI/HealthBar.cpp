#include "GUI/HealthBar.h"
#include "ResourceManager.h"
#include "GUI/GUIConstants.h"
#include "GameObjectComponents/TooltipWindowComponent.h"
#include "GlobalResource.h"

HealthBar::HealthBar(const AttributeData* attributes, HealthBarStyle style) {
	m_isInputInDefaultView = true;
	m_boundingBox.height = BAR_HEIGHT;
	setDebugBoundingBox(sf::Color::Yellow);
	sf::Vector2f initialPosition;

	if (style == HealthBarStyle::MainCharacter) {
		m_boundingBox.width = 300.f;
		
		m_borderOffsetX = 13.f;
		m_borderOffsetY = 10.f;

		m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_MAINCHAR_BORDER);

		initialPosition = sf::Vector2f(18.f, 18.f);
	}
	else if (style == HealthBarStyle::Enemy) {
		m_boundingBox.width = 200.f;
		
		m_borderOffsetX = 8.f;
		m_borderOffsetY = 6.f;

		m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_ENEMY_BORDER);

		initialPosition = sf::Vector2f(WINDOW_WIDTH - m_boundingBox.width - 2.f * 18.f, 18.f);
	}
	else if (style == HealthBarStyle::Boss) {
		m_boundingBox.width = 300.f;

		m_borderOffsetX = 12.f;
		m_borderOffsetY = 10.f;

		m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_BOSS_BORDER);

		initialPosition = sf::Vector2f(WINDOW_WIDTH - m_boundingBox.width - 2.f * 18.f, 18.f);
	}

	m_barTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT);
	m_hitOverlayTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT_HIT);
	m_hitOverlayHighlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT_HIGHLIGHT);

	// init bar
	m_bar.setTexture(m_barTexture);
	m_bar.setSize(sf::Vector2f(0.f, BAR_HEIGHT));

	// init border
	m_border.setSize(sf::Vector2f(m_boundingBox.width + 2.f * m_borderOffsetX, BAR_HEIGHT + 2.f * m_borderOffsetY));
	m_border.setTexture(m_borderTexture);

	// init overlay
	m_hitOverlay.setSize(sf::Vector2f(0.f, 0.f));
	m_hitOverlay.setTexture(m_hitOverlayTexture);

	m_highlightTime = sf::Time::Zero;
	m_waitTime = sf::Time::Zero;
	m_shrinkTime = sf::Time::Zero;

	m_tooltipComponent = new TooltipWindowComponent("", this);
	m_tooltipComponent->setWindowOffset(sf::Vector2f(0.5f * m_boundingBox.width, BAR_HEIGHT + 10.f));
	m_tooltipComponent->setTextAlignment(TextAlignment::Center);
	addComponent(m_tooltipComponent);

	setAttributes(attributes);

	m_name.setTextAlignment(TextAlignment::Center);
	m_name.setTextStyle(TextStyle::Shadowed);
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	setName("");

	setPosition(initialPosition);
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
	m_name.setString(g_textProvider->getCroppedString(name, m_name.getCharacterSize(), static_cast<int>(m_boundingBox.width + 20.f)));
	setPosition(getPosition());
}

void HealthBar::setVisible(bool visible) {
	m_isVisible = visible;
}

void HealthBar::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	GameObject::render(target);
	target.draw(m_bar);
	target.draw(m_hitOverlay);
	target.draw(m_border);
	target.draw(m_name);
}

void HealthBar::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);

	m_bar.setPosition(pos);
	m_border.setPosition(pos - sf::Vector2f(m_borderOffsetX, m_borderOffsetY));
	m_hitOverlay.setPosition(pos);
	auto const bounds = m_name.getBounds();
	m_name.setPosition(pos + sf::Vector2f(0.5f * (m_boundingBox.width - bounds.width), BAR_HEIGHT + 10.f));
}


void HealthBar::update(const sf::Time& frameTime) {
	if (m_attributes == nullptr) return;
	GameObject::update(frameTime);

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
	float normalWidth = m_boundingBox.width * (static_cast<float>(m_currentHP) / m_attributes->maxHealthPoints);
	m_bar.setSize(sf::Vector2f(normalWidth, BAR_HEIGHT));

	// Set overlay
	float overlayX = m_boundingBox.left + normalWidth;
	float overlayWidth = std::max(0.f, m_boundingBox.width * (static_cast<float>(m_overlayHP - m_currentHP) / m_attributes->maxHealthPoints));
	m_hitOverlay.setPosition(sf::Vector2f(overlayX, m_boundingBox.top));
	m_hitOverlay.setSize(sf::Vector2f(overlayWidth, BAR_HEIGHT));

	// Update tooltip
	m_tooltipComponent->setTooltipText(std::to_string(m_currentHP) + "/" + std::to_string(m_attributes->maxHealthPoints));
}