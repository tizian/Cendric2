#include "GameObjectComponents/TooltipComponent.h"

const sf::Time TooltipComponent::TOOLTIP_TIME = sf::seconds(1.f);
const float TooltipComponent::TOOLTIP_HEIGHT = 10.f;

TooltipComponent::TooltipComponent(std::string tooltip, AnimatedGameObject* parent, bool useInteractiveColor) : GameObjectComponent(parent) {
	m_animatedParent = parent;
	m_useInteractiveColor = useInteractiveColor;

	m_tooltipText.setString(tooltip);
	m_tooltipText.setTextStyle(TextStyle::Shadowed);
	m_tooltipText.setColor(COLOR_WHITE);
	m_tooltipText.setCharacterSize(8);
}

void TooltipComponent::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_tooltipTime, frameTime);
}

void TooltipComponent::setPosition(const sf::Vector2f& pos) {
	m_tooltipText.setPosition(sf::Vector2f(pos.x, pos.y - TOOLTIP_HEIGHT));
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
