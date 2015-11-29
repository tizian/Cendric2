#include "GUI/BuffBar.h"

const int BuffBar::MAX_SHOWABLE_BUFFSLOTS = 5;

BuffBar::BuffBar() {
}

BuffBar::~BuffBar() {
	delete m_foodBuffSlot;
	for (auto& it : m_buffSlots) {
		delete it;
	}
	m_buffSlots.clear();
}

void BuffBar::show() {
	m_isVisible = true;
}

void BuffBar::hide() {
	m_isVisible = false;
}

void BuffBar::addSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration, SpellID id) {
	if (type == BuffType::Food) {
		delete m_foodBuffSlot;
		m_foodBuffSlot = new BuffSlot(type, textureLocation, duration, SpellID::VOID);
	}
	else {
		m_buffSlots.push_back(new BuffSlot(type, textureLocation, duration, id));
	}
	calculateSlotPositions();
}

void BuffBar::removeTypedSlots(SpellID id) {
	for (auto& slot : m_buffSlots) {
		if (slot->getSpellID() == id) {
			slot->setDisposed();
		}
	}
}

void BuffBar::render(sf::RenderTarget& target) {
	if (m_foodBuffSlot != nullptr) m_foodBuffSlot->render(target);
	for (int i = 0; i < static_cast<int>(m_buffSlots.size()); i++) {
		m_buffSlots.at(i)->render(target);
		if (i >((m_foodBuffSlot == nullptr) ? MAX_SHOWABLE_BUFFSLOTS - 1 : MAX_SHOWABLE_BUFFSLOTS - 2)) break;
	}
}

void BuffBar::update(const sf::Time& frameTime) {
	if (m_foodBuffSlot != nullptr) {
		m_foodBuffSlot->update(frameTime);
		if (m_foodBuffSlot->isDisposed()) {
			delete m_foodBuffSlot;
			m_foodBuffSlot = nullptr;
			calculateSlotPositions();
		}
	}

	for (auto it = m_buffSlots.begin(); it != m_buffSlots.end(); /*don't increment here*/) {
		(*it)->update(frameTime);
		if ((*it)->isDisposed()) {
			delete (*it);
			it = m_buffSlots.erase(it);
			calculateSlotPositions();
		}
		else {
			it++;
		}
	}
}

void BuffBar::calculateSlotPositions() {
	// the foodbuff is always the first one (as seen from left)
	sf::Vector2f offset = BUFFBAR_OFFSET;
	float xOffset = BUFFSLOT_SPACING + 2 * BuffSlot::RADIUS + 2 * BuffSlot::MARGIN;
	if (m_foodBuffSlot != nullptr) {
		m_foodBuffSlot->setPosition(offset);
		offset.x += xOffset;
	}
	for (auto& it : m_buffSlots) {
		it->setPosition(offset);
		offset.x += xOffset;
	}
}