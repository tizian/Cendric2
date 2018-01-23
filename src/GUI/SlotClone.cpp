#include "GUI/SlotClone.h" 

SlotClone::SlotClone(const Slot* original) {
	m_original = original;

	float size = original->getConfiguredSize();
	setBoundingBox(sf::FloatRect(0.f, 0.f, size, size));
	setDebugBoundingBox(COLOR_BAD);
	setInputInDefaultView(true);

	m_backgroundRect.setSize(sf::Vector2f(Slot::ICON_SIZE, Slot::ICON_SIZE));
	m_backgroundRect.setFillColor(COLOR_TRANS_GREY);

	m_iconRect.setSize(sf::Vector2f(Slot::ICON_SIZE, Slot::ICON_SIZE));
	m_iconRect.setTexture(original->m_iconTexture);
	m_iconRect.setTextureRect(original->m_iconTextureRect);
	m_iconRect.setFillColor(original->m_iconRect.getFillColor());

	m_borderRect.setSize(sf::Vector2f(size, size));
	m_borderRect.setTexture(original->m_borderTextureSelected);
}

GameObjectType SlotClone::getConfiguredType() const {
	return GameObjectType::_Interface;
}

const Slot* SlotClone::getOriginalSlot() const {
	return m_original;
}

void SlotClone::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	float iconOffset = m_original->getConfiguredIconOffset();
	m_backgroundRect.setPosition(pos.x + iconOffset, pos.y + iconOffset);
	m_iconRect.setPosition(pos.x + iconOffset, pos.y + iconOffset);
	m_borderRect.setPosition(pos);
}

void SlotClone::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backgroundRect);
	renderTarget.draw(m_iconRect);
	renderTarget.draw(m_borderRect);
}
