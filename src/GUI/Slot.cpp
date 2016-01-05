#include "GUI/Slot.h"

using namespace std;

const float Slot::ICON_SIZE = 50.f;

void Slot::initSlot() {
	float size = getConfiguredSize();
	setBoundingBox(sf::FloatRect(0.f, 0.f, size, size));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_backgroundRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_backgroundRect.setFillColor(CENDRIC_COLOR_TRANS_GREY);

	m_iconRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_iconRect.setTexture(m_iconTexture);
	m_iconRect.setTextureRect(m_iconTextureRect);

	m_overlayRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_overlayRect.setFillColor(CENDRIC_COLOR_TRANSPARENT);	

	m_borderRect.setSize(sf::Vector2f(size, size));
	m_borderRect.setTexture(m_borderTexture);

	if (m_isEmpty) {
		m_borderRect.setFillColor(CENDRIC_COLOR_MEDIUM_GREY);
	}
}

void Slot::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	float iconOffset = getConfiguredIconOffset();
	m_backgroundRect.setPosition(pos.x + iconOffset, pos.y + iconOffset);
	m_iconRect.setPosition(pos.x + iconOffset, pos.y + iconOffset);
	m_overlayRect.setPosition(pos.x + iconOffset, pos.y + iconOffset);
	m_borderRect.setPosition(pos);
}

void Slot::activate() {
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 0));
	if (m_isSelected) {
		m_borderRect.setTexture(m_borderTextureSelected);
	}
	else {
		m_borderRect.setTexture(m_borderTexture);
	}
}

void Slot::deactivate() {
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 128));
	m_borderRect.setTexture(m_borderTexture);
}

void Slot::select() {
	if (m_isSelected || m_isEmpty) return;
	m_isSelected = true;
	m_borderRect.setTexture(m_borderTextureSelected);
}

void Slot::deselect() {
	if (!m_isSelected || m_isEmpty) return;
	m_isSelected = false;
	m_borderRect.setTexture(m_borderTexture);
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
	GameObject::update(frameTime);
}

void Slot::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backgroundRect);
	renderTarget.draw(m_iconRect);
	renderTarget.draw(m_overlayRect);
	renderTarget.draw(m_borderRect);
}

void Slot::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
}

void Slot::onLeftJustPressed() {
	if (m_isEmpty) return;
	m_isClicked = true;
	g_inputController->lockAction();
}

void Slot::onRightClick() {
	if (m_isEmpty) return;
	m_isRightClicked = true;
	g_inputController->lockAction();
}

GameObjectType Slot::getConfiguredType() const {
	return GameObjectType::_Interface;
}