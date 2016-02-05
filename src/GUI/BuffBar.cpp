#include "GUI/BuffBar.h"
#include "Level/LevelInterface.h"

const int BuffBar::MAX_SHOWABLE_BUFFSLOTS = 5;

BuffBar::BuffBar(LevelInterface* _interface) {
	m_interface = _interface;
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

void BuffBar::addSpellBuff(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr) {
	BuffSlot* buff = new BuffSlot(BuffType::Spell, textureLocation, duration);
	buff->setSpellAttributes(spell, attr);
	m_buffSlots.push_back(buff);
	m_notifyInterface = true;
	calculateBuffPositions();
}

void BuffBar::addFoodBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr) {
	delete m_foodBuffSlot;
	m_foodBuffSlot = new BuffSlot(BuffType::Food, textureLocation, duration);
	m_foodBuffSlot->setFoodAttributes(itemID, attr);
	m_notifyInterface = true;
	calculateBuffPositions();
}

void BuffBar::addDotBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data) {
	BuffSlot* buff = new BuffSlot(BuffType::DamageOverTime, textureLocation, duration);
	buff->setDotAttributes(data);
	m_buffSlots.push_back(buff);
	m_notifyInterface = true;
	calculateBuffPositions();
}

void BuffBar::removeTypedSpellBuffs(SpellID id) {
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
	if (m_foodBuffSlot != nullptr) m_foodBuffSlot->renderAfterForeground(target);
	for (int i = 0; i < static_cast<int>(m_buffSlots.size()); i++) {
		m_buffSlots.at(i)->renderAfterForeground(target);
		if (i >((m_foodBuffSlot == nullptr) ? MAX_SHOWABLE_BUFFSLOTS - 1 : MAX_SHOWABLE_BUFFSLOTS - 2)) break;
	}
}

void BuffBar::update(const sf::Time& frameTime) {
	if (m_notifyInterface) {
		m_interface->notifyCharacterInfo();
		m_notifyInterface = false;
	}
	if (m_foodBuffSlot != nullptr) {
		m_foodBuffSlot->update(frameTime);
		if (m_foodBuffSlot->isDisposed()) {
			delete m_foodBuffSlot;
			m_foodBuffSlot = nullptr;
			m_notifyInterface = true;
			calculateBuffPositions();
		}
	}

	for (auto it = m_buffSlots.begin(); it != m_buffSlots.end(); /*don't increment here*/) {
		(*it)->update(frameTime);
		if ((*it)->isDisposed()) {
			delete (*it);
			it = m_buffSlots.erase(it);
			m_notifyInterface = true;
			calculateBuffPositions();
		}
		else {
			it++;
		}
	}
}

void BuffBar::calculateBuffPositions() {
	// the foodbuff is always the first one (as seen from left)
	sf::Vector2f offset = BUFFBAR_OFFSET;
	float xOffset = BUFFSLOT_SPACING + BuffSlot::SIZE;
	if (m_foodBuffSlot != nullptr) {
		m_foodBuffSlot->setPosition(offset);
		offset.x += xOffset;
	}
	for (auto& it : m_buffSlots) {
		it->setPosition(offset);
		offset.x += xOffset;
	}
}