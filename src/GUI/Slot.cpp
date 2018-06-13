#include "GUI/Slot.h"
#include "GameObjectComponents/TooltipWindowComponent.h"

const float Slot::ICON_SIZE = 50.f;
const float Slot::TOOLTIP_TOP = 10.f;
const sf::Time Slot::DOUBLE_CLICK_TIME = sf::milliseconds(500);

Slot::Slot() {
	m_tooltipComponent = new TooltipWindowComponent("", this);
	addComponent(m_tooltipComponent);
}

void Slot::initSlot() {
	const auto size = getConfiguredSize();
	setBoundingBox(sf::FloatRect(0.f, 0.f, ICON_SIZE, ICON_SIZE));
	setDebugBoundingBox(COLOR_BAD);
	setInputInDefaultView(true);

	m_backgroundRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_backgroundRect.setFillColor(COLOR_TRANS_GREY);

	m_iconRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_iconRect.setTexture(m_iconTexture);
	m_iconRect.setTextureRect(m_iconTextureRect);

	m_overlayRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_overlayRect.setFillColor(COLOR_TRANSPARENT);

	m_borderRect.setSize(sf::Vector2f(size, size));
	m_borderRect.setTexture(m_borderTexture);

	if (m_isEmpty) {
		m_borderRect.setFillColor(COLOR_MEDIUM_GREY);
	}
}

void Slot::setPosition(const sf::Vector2f& pos) {
	adjustTooltipOffset();
	GameObject::setPosition(pos);
	const auto iconOffset = getConfiguredIconOffset();
	m_backgroundRect.setPosition(pos);
	m_iconRect.setPosition(pos);
	m_overlayRect.setPosition(pos);
	m_borderRect.setPosition(pos.x - iconOffset, pos.y - iconOffset);
}

void Slot::adjustTooltipOffset() {
	m_tooltipComponent->setWindowOffset(sf::Vector2f(0.f, -m_tooltipComponent->getHeight() - TOOLTIP_TOP));
}

void Slot::activate() {
	m_isActive = true;
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 0));
	if (m_isSelected) {
		m_borderRect.setTexture(m_borderTextureSelected);
		m_backgroundRect.setFillColor(COLOR_TRANS_WHITE);
	}
	else {
		m_borderRect.setTexture(m_borderTexture);
		m_backgroundRect.setFillColor(COLOR_TRANS_GREY);
	}
}

void Slot::deactivate() {
	m_isActive = false;
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 128));
	m_borderRect.setTexture(m_borderTexture);
	m_backgroundRect.setFillColor(COLOR_TRANS_GREY);
}

void Slot::click() {
	select();
}

void Slot::updateColor() {
	m_borderRect.setTexture(m_isSelected ? m_borderTextureSelected : m_borderTexture);
	m_backgroundRect.setFillColor(m_isSelected && !m_isEmpty ? COLOR_TRANS_WHITE : COLOR_TRANS_GREY);
	m_tooltipComponent->setCurrentTooltipTime(m_isSelected ? sf::seconds(2.f) : sf::Time::Zero);
}

void Slot::notifyFirstSelection() {
	m_tooltipComponent->setCurrentTooltipTime(sf::Time::Zero);
}

void Slot::select() {
	if (m_isEmpty || m_isSelected) return;
	m_isSelected = true;
	m_borderRect.setTexture(m_borderTextureSelected);
	m_backgroundRect.setFillColor(COLOR_TRANS_WHITE);
}

void Slot::deselect() {
	if (!m_isSelected) return;
	m_isSelected = false;
	m_borderRect.setTexture(m_borderTexture);
	m_backgroundRect.setFillColor(COLOR_TRANS_GREY);
}

void Slot::highlight() {
	m_overlayRect.setFillColor(sf::Color(255, 255, 255, 150));
	m_overlayRect.setTexture(m_highlightTexture);
}

void Slot::unhighlight() {
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 0));
	m_overlayRect.setTexture(nullptr);
}

void Slot::update(const sf::Time& frameTime) {
	m_isClicked = false;
	m_isRightClicked = false;
	m_isDoubleClicked = false;
	m_isMouseOver = false;
	updateTime(m_doubleClickTime, frameTime);
	GameObject::update(frameTime);
}

void Slot::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backgroundRect);
	renderTarget.draw(m_iconRect);
	renderTarget.draw(m_overlayRect);
	renderTarget.draw(m_borderRect);
}

void Slot::onLeftJustPressed() {
	if (m_isEmpty) return;
	m_isClicked = true;
	if (m_doubleClickTime > sf::Time::Zero) {
		m_isDoubleClicked = true;
		m_doubleClickTime = sf::Time::Zero;
		g_inputController->lockAction();
		return;
	}
	m_doubleClickTime = DOUBLE_CLICK_TIME;
	g_inputController->lockAction();
}

void Slot::onRightClick() {
	if (m_isEmpty) return;
	m_isRightClicked = true;
	g_inputController->lockAction();
}

void Slot::onMouseOver() {
	GameObject::onMouseOver();
	m_isMouseOver = true;
}

GameObjectType Slot::getConfiguredType() const {
	return _Interface;
}