#include "GUI/BuffBar.h"
#include "Level/LevelInterface.h"
#include "Screens/WorldScreen.h"

const int BuffBar::MAX_SHOWABLE_BUFFSLOTS = 5;

BuffBar::BuffBar(LevelInterface* _interface) {
	m_interface = _interface;
}

BuffBar::~BuffBar() {
	CLEAR_MAP(m_buffSlots);
}

void BuffBar::show() {
	m_isVisible = true;
}

void BuffBar::hide() {
	m_isVisible = false;
}

void BuffBar::addSpellBuff(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr) {
	BuffSlot* buff;
	int spellId = static_cast<int>(spell->getSpellID());
	if (contains(m_buffSlots, spellId)) {
		buff = m_buffSlots.at(spellId);
	}
	else {
		buff = new BuffSlot(BuffSlotType::Spell, textureLocation);
		m_buffSlots.insert({spellId, buff });
	}
	
	buff->setSpellAttributes(spell, attr, duration);
	m_notifyInterface = true;
	calculateBuffPositions();
}

void BuffBar::addFoodBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr) {
	BuffSlot* buff;
	int buffType = static_cast<int>(BuffType::Food);
	if (contains(m_buffSlots, buffType)) {
		buff = m_buffSlots.at(buffType);
	}
	else {
		buff = new BuffSlot(BuffSlotType::Food, textureLocation);
		m_buffSlots.insert({ buffType, buff });
	}
	
	buff->setFoodAttributes(itemID, attr, duration);
	m_notifyInterface = true;
	calculateBuffPositions();
}

void BuffBar::addDebuffBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data) {
	if (!data.isFeared && !data.isStunned) return;
	
	BuffSlot* buff;

	int buffType = static_cast<int>(data.isFeared ? BuffType::Fear : BuffType::Stun);
	if (contains(m_buffSlots, buffType)) {
		buff = m_buffSlots.at(buffType);
	}
	else {
		buff = new BuffSlot(BuffSlotType::Debuff, textureLocation);
		m_buffSlots.insert({ buffType, buff });
	}

	buff->setScreen(m_interface->getScreen());
	buff->setDebuffAttributes(data, duration);
	m_notifyInterface = true;
	calculateBuffPositions();
}


void BuffBar::addDotBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data) {
	if (data.damageType <= DamageType::VOID || data.damageType >= DamageType::MAX) return;

	BuffSlot* buff;
	int buffType;

	switch (data.damageType) {
	case DamageType::Fire:
		buffType = static_cast<int>(BuffType::FireDamage);
		break;
	case DamageType::Ice:
		buffType = static_cast<int>(BuffType::IceDamage);
		break;
	case DamageType::Light:
		buffType = static_cast<int>(BuffType::LightDamage);
		break;
	case DamageType::Shadow:
		buffType = static_cast<int>(BuffType::ShadowDamage);
		break;
	case DamageType::Physical:
	default:
		buffType = static_cast<int>(BuffType::PhysicalDamage);
		break;
	}

	if (contains(m_buffSlots, buffType)) {
		buff = m_buffSlots.at(buffType);
	}
	else {
		buff = new BuffSlot(BuffSlotType::Dot, textureLocation);
		m_buffSlots.insert({ buffType, buff });
	}
	
	buff->setScreen(m_interface->getScreen());
	buff->setDotAttributes(data, duration);
	m_notifyInterface = true;
	calculateBuffPositions();
}

void BuffBar::removeTypedSpellBuffs(SpellID id) {
	auto spellId = static_cast<int>(id);
	if (!contains(m_buffSlots, spellId)) return;

	auto& buff = m_buffSlots.at(spellId);
	buff->setDisposed();
}

void BuffBar::removeFoodBuff() {
	auto id = static_cast<int>(BuffType::Food);
	if (!contains(m_buffSlots, id)) return;

	auto& buff = m_buffSlots.at(id);
	buff->setDisposed();
}

void BuffBar::render(sf::RenderTarget& target) {
	int i = 0;
	for (auto& it : m_buffSlots) {
		it.second->render(target);
		i++;
		if (i > MAX_SHOWABLE_BUFFSLOTS - 1) break;
	}

	i = 0;
	for (auto& it : m_buffSlots) {
		it.second->renderAfterForeground(target);
		i++;
		if (i > MAX_SHOWABLE_BUFFSLOTS - 1) break;
	}
}

void BuffBar::update(const sf::Time& frameTime) {
	if (m_notifyInterface) {
		m_interface->notifyCharacterInfo();
		m_notifyInterface = false;
	}
	
	for (auto it = m_buffSlots.begin(); it != m_buffSlots.end(); /*don't increment here*/) {
		(*it).second->update(frameTime);
		if ((*it).second->isDisposed()) {
			delete (*it).second;
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
	sf::Vector2f offset = BUFFBAR_OFFSET;
	float xOffset = BUFFSLOT_SPACING + BuffSlot::SIZE;
	for (auto& it : m_buffSlots) {
		it.second->setPosition(offset);
		offset.x += xOffset;
	}
}