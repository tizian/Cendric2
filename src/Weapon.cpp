#include "Weapon.h"
#include "Logger.h"

Weapon::Weapon(const ItemBean& bean) : Item(bean)
{
	m_weaponSlots.clear();
	for (auto& it : bean.weaponSlots)
	{
		std::pair<SpellType, SpellID> emptySpellSlot(it.type, SpellID::VOID);
		std::map<SpellModifierType, SpellModifier> modifiers;
		// insert modifier slots
		for (auto& it2 : it.allowedModifiers)
		{
			modifiers.insert({it2, EMPTY_SPELLMODIFIER});
		}
		
		std::pair<
			std::pair<SpellType, SpellID>,
			std::map<SpellModifierType, SpellModifier>> slot(emptySpellSlot, modifiers);
		
		m_weaponSlots.push_back(slot);
	}
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
	return &m_weaponSlots[slotNr].second;
}

bool Weapon::addModifier(int slotNr, const SpellModifier& modifier, bool force)
{
	if (!isModifierAllowed(slotNr, modifier.type))
	{
		g_logger->logError("Weapon::addModifier", "This slot doesn't exist or doesn't allow a modifier of this type");
		return false;
	}
	// check if this slot is already taken
	if (m_weaponSlots.at(slotNr).second.at(modifier.type).type != SpellModifierType::VOID)
	{
		g_logger->logWarning("Weapon::addModifier", "This modifier slot is already taken");
		if (!force) return false;
	}

	m_weaponSlots.at(slotNr).second[modifier.type] = modifier;
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

bool Weapon::isModifierAllowed(int slotNr, SpellModifierType modifier) const
{
	// check if slot with this nr exists (avoid out-of-range exception)
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1)
	{
		g_logger->logError("Weapon", "This weapon has not enough slots for the modification (slots: " + std::to_string(m_weaponSlots.size()) + ", required nr: " + std::to_string(slotNr));
		return false;
	}
	// check if modifier is allowed
	const std::map<SpellModifierType, SpellModifier>& modifierSlots = m_weaponSlots.at(slotNr).second;
	return (modifierSlots.find(modifier) != modifierSlots.end());
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