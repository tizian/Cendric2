#include "Weapon.h"
#include "Logger.h"

Weapon::Weapon(const ItemBean& bean) : Item(bean)
{
	m_weaponSlots.clear();
	for (auto& it : bean.weaponSlots)
	{
		std::pair<SpellType, SpellID> emptySpellSlot(it.type, SpellID::VOID);
		std::pair<int, std::map<SpellModifierType, SpellModifier>> modifiers;
		modifiers.first = it.modifierCount;
		
		std::pair<
			std::pair<SpellType, SpellID>,
			std::pair<
			int,
			std::map<SpellModifierType, SpellModifier >>> slot(emptySpellSlot, modifiers);
		
		m_weaponSlots.push_back(slot);
	}
}

const std::vector < std::pair <
	std::pair<SpellType, SpellID>,
	std::pair<
	int,
	std::map < SpellModifierType, SpellModifier >> >>& Weapon::getWeaponSlots() const
{
	return m_weaponSlots;
}

const sf::Time& Weapon::getWeaponCooldown() const
{
	return m_bean.weaponChopCooldown;
}

const sf::FloatRect& Weapon::getWeaponChopRect() const
{
	return m_bean.weaponChopRect;
}

int Weapon::getWeaponChopDamage() const
{
	return m_bean.weaponChopDamage;
}

SpellID Weapon::getCurrentSpellForSlot(int slotNr) const
{
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1)
	{
		return SpellID::VOID;
	}
	return m_weaponSlots[slotNr].first.second;
}

const std::map<SpellModifierType, SpellModifier>* Weapon::getCurrentModifiersForSlot(int slotNr) const
{
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1)
	{
		return nullptr;
	}
	return &m_weaponSlots[slotNr].second.second;
}

bool Weapon::addModifier(int slotNr, const SpellModifier& modifier, bool force)
{
	if (!doesSlotExist(slotNr)) return false;
	
	std::map<SpellModifierType, SpellModifier>& modifiers = m_weaponSlots.at(slotNr).second.second;

	// check if this slot is already taken
	if (modifiers.find(modifier.type) != modifiers.end())
	{
		g_logger->logWarning("Weapon::addModifier", "The modifier slot for this type is already taken");
		if (!force) return false;
	}
	// if not, check if there is enough space for the new modifier
	else if (modifiers.size() >= m_weaponSlots.at(slotNr).second.first)
	{
		g_logger->logError("Weapon::addModifier", "There is not enough space for a new modifier!");
		return false;
	}

	m_weaponSlots.at(slotNr).second.second[modifier.type] = modifier;
	return true;
}

bool Weapon::addSpell(int slotNr, SpellID id, bool force)
{
	if (!isSpellAllowed(slotNr, id))
	{
		g_logger->logError("Weapon::addSpell", "This slot doesn't exist or doesn't allow a spell of this type");
		return false;
	}
	// check if this slot is already taken
	if (m_weaponSlots.at(slotNr).first.second != SpellID::VOID)
	{
		g_logger->logWarning("Weapon::addSpell", "This spell slot is already taken");
		if (!force) return false;
	}

	m_weaponSlots.at(slotNr).first.second = id;
	return true;
}

bool Weapon::doesSlotExist(int slotNr) const
{
	// check if slot with this nr exists 
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1)
	{
		g_logger->logError("Weapon", "This weapon has not enough slots for the modification (slots: " + std::to_string(m_weaponSlots.size()) + ", required nr: " + std::to_string(slotNr));
		return false;
	}
	return true;
}

bool Weapon::isSpellAllowed(int slotNr, SpellID id) const
{
	// check if slot with this nr exists
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1)
	{
		g_logger->logError("Weapon", "This weapon has not enough slots for the modification (slots: " + std::to_string(m_weaponSlots.size()) + ", required nr: " + std::to_string(slotNr));
		return false;
	}
	// check if this spell bean has the correct type for this 
	return (m_weaponSlots.at(slotNr).first.first == SpellBean::getSpellBean(id).spellType);
}