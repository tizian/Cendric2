#include "GameObjectComponents/TooltipWindowComponent.h"
#include "GUI/GUIConstants.h"

TooltipWindowComponent::TooltipWindowComponent(const std::string& tooltip, GameObject* parent) : GameObjectComponent(parent) {	
	m_tooltipWindow.setText(tooltip);
}

void TooltipWindowComponent::update(const sf::Time& frameTime) {
	GameObjectComponent::update(frameTime);
	updateTime(m_currentTooltipTime, frameTime);
}

void TooltipWindowComponent::setPosition(const sf::Vector2f& pos) {
	m_tooltipWindow.setPosition(pos + m_offset);
}

void TooltipWindowComponent::setTextAlignment(TextAlignment alignment) {
	m_tooltipWindow.setTextAlignment(alignment);
}

void TooltipWindowComponent::setTextOffset(const sf::Vector2f& offset) {
	m_tooltipWindow.setTextOffset(offset);
}

void TooltipWindowComponent::setWindowOffset(const sf::Vector2f& offset) {
	m_offset = offset;
	setPosition(m_parent->getPosition());
}

void TooltipWindowComponent::setMaxWidth(int width) {
	m_tooltipWindow.setMaxWidth(width);
}

void TooltipWindowComponent::setTooltipText(const std::string& tooltip) {
	m_tooltipWindow.setText(tooltip);
	setPosition(m_parent->getPosition());
}

void TooltipWindowComponent::setTooltipTime(const sf::Time& time) {
	m_tooltipTime = time;
}

void TooltipWindowComponent::setShowOnTooltipToggle(bool show) {
	m_showOnTooltipToggle = show;
}

void TooltipWindowComponent::setCurrentTooltipTime(const sf::Time& time) {
	m_currentTooltipTime = time;
}

void TooltipWindowComponent::renderAfterForeground(sf::RenderTarget& renderTarget) {
	if (isShowingTooltip()) {
		m_tooltipWindow.render(renderTarget);
		m_showTooltip = false;
	}
}

void TooltipWindowComponent::onParentMouseOver() {
	m_showTooltip = true;
	m_currentTooltipTime = m_tooltipTime;
}

float TooltipWindowComponent::getWidth() const {
	return m_tooltipWindow.getSize().x;
}

float TooltipWindowComponent::getHeight() const {
	return m_tooltipWindow.getSize().y;
}

bool TooltipWindowComponent::isShowingTooltip() const {
	return (m_showOnTooltipToggle && g_inputController->isKeyActive(Key::ToggleTooltips)) || 
		m_showTooltip || m_currentTooltipTime > sf::Time::Zero;
}
