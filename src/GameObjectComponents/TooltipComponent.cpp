#include "GameObjectComponents/TooltipComponent.h"
#include "GUI/GUIConstants.h"

const sf::Time TooltipComponent::TOOLTIP_TIME = sf::seconds(1.f);

TooltipComponent::TooltipComponent(const std::string& tooltip, AnimatedGameObject* parent, bool useInteractiveColor) : GameObjectComponent(parent) {
	m_animatedParent = parent;
	m_useInteractiveColor = useInteractiveColor;

	m_tooltipText.setString(tooltip);
	m_tooltipText.setTextStyle(TextStyle::Shadowed);
	m_tooltipText.setColor(COLOR_WHITE);
	m_tooltipText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);

	m_tooltipHeight = 10.f;
}

void TooltipComponent::update(const sf::Time& frameTime) {
	updateTime(m_tooltipTime, frameTime);
}

void TooltipComponent::setPosition(const sf::Vector2f& pos) {
	m_tooltipText.setPosition(pos + sf::Vector2f(
		0.5f * (m_parent->getBoundingBox()->width - m_tooltipText.getLocalBounds().width), 
		-m_tooltipHeight));
}

void TooltipComponent::setTooltipHeight(float height) {
	m_tooltipHeight = height;
}

void TooltipComponent::setTooltipText(const std::string& tooltip) {
	m_tooltipText.setString(tooltip);
	setPosition(m_parent->getPosition());
}

void TooltipComponent::renderAfterForeground(sf::RenderTarget& renderTarget) {
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}

void TooltipComponent::onParentMouseOver() {
	if (m_useInteractiveColor) {
		m_animatedParent->setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	}
	m_tooltipTime = TOOLTIP_TIME;
}
