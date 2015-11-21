#include "Weapon.h"
#include "Logger.h"

Weapon::Weapon(const ItemBean& bean) : Item(bean) {
	reload();
}

void Weapon::reload() {
	m_weaponSlots.clear();
	for (auto& it : m_bean.weaponSlots) {
		SpellSlot_s emptySpellSlot;
		emptySpellSlot.spellType = it.type;
		emptySpellSlot.spellID = SpellID::VOID;

		std::vector<SpellModifier> modifiers;
		for (int i = 0; i < it.modifierCount; i++) {
			SpellModifier modifier;
			modifier.type = SpellModifierType::VOID;
			modifiers.push_back(modifier);
		}

		WeaponSlot_s slot;
		slot.spellSlot = emptySpellSlot;
		slot.spellModifiers = modifiers;

		m_weaponSlots.push_back(slot);
	}
}

const std::vector<WeaponSlot_s>& Weapon::getWeaponSlots() const {
	return m_weaponSlots;
}

const sf::Time& Weapon::getWeaponCooldown() const {
	return m_bean.weaponChopCooldown;
}

const sf::FloatRect& Weapon::getWeaponChopRect() const {
	return m_bean.weaponChopRect;
}

int Weapon::getWeaponChopDamage() const {
	return m_bean.weaponChopDamage;
}

SpellID Weapon::getCurrentSpellForSlot(int slotNr) const {
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1) {
		return SpellID::VOID;
	}
	return m_weaponSlots[slotNr].spellSlot.spellID;
}

const std::vector<SpellModifier>* Weapon::getCurrentModifiersForSlot(int slotNr) const {
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1) {
		return nullptr;
	}
	return &m_weaponSlots[slotNr].spellModifiers;
}

bool Weapon::addModifier(int slotNr, int modifierNr, const SpellModifier& modifier, bool force) {
	if (!doesSlotExist(slotNr)) return false;

	std::vector<SpellModifier>& modifiers = m_weaponSlots.at(slotNr).spellModifiers;

	// check if there is a valid spell in this slot
	if (m_weaponSlots.at(slotNr).spellSlot.spellID == SpellID::VOID) {
		g_logger->logWarning("Weapon::addModifier", "Cannot add a modifier to an empty spellslot");
		return false;
	}

	// check if this modifierNr is allowed
	if (modifierNr > modifiers.size() - 1 || modifierNr < 0) {
		g_logger->logWarning("Weapon::addModifier", "Modifier slot with nr " + std::to_string(modifierNr) + " does not exist!");
		return false;
	}

	if (!force) {
		// check if this particular slot is already taken
		if (modifiers[modifierNr].type != SpellModifierType::VOID) {
			if (!force) {
				g_logger->logWarning("Weapon::addModifier", "This modifier slot is already taken");
				return false;
			}
		}

		// check if this slot for this type is already taken
		for (auto& mod : modifiers) {
			if (mod.type == modifier.type) {
				g_logger->logWarning("Weapon::addModifier", "The modifier slot for this type is already taken");
				return false;
			}
		}
	}

	// check if this spell allows a modifier of this type
	std::vector<SpellModifierType> allowedModifiers = SpellBean::getAllowedModifiers(m_weaponSlots.at(slotNr).spellSlot.spellID);
	if (std::find(allowedModifiers.begin(), allowedModifiers.end(), modifier.type) == allowedModifiers.end()) {
		g_logger->logWarning("Weapon::addModifier", "This modifier is not allowed for the spell!");
		return false;
	}

	// remove the old modifier of this type, if any
	for (auto& mod : modifiers) {
		if (mod.type == modifier.type) {
			mod.type = SpellModifierType::VOID;
		}
	}

	m_weaponSlots.at(slotNr).spellModifiers[modifierNr] = modifier;
	return true;
}

bool Weapon::addSpell(int slotNr, SpellID id, bool force) {
	if (!isSpellAllowed(slotNr, id)) {
		g_logger->logError("Weapon::addSpell", "This slot doesn't exist or doesn't allow a spell of this type");
		return false;
	}
	// check if this slot is already taken
	if (m_weaponSlots.at(slotNr).spellSlot.spellID != SpellID::VOID) {
		g_logger->logWarning("Weapon::addSpell", "This spell slot is already taken");
		if (!force) return false;
	}

	m_weaponSlots.at(slotNr).spellSlot.spellID = id;
	return true;
}

bool Weapon::doesSlotExist(int slotNr) const {
	// check if slot with this nr exists 
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1) {
		g_logger->logError("Weapon", "This weapon has not enough slots for the modification (slots: " + std::to_string(m_weaponSlots.size()) + ", required nr: " + std::to_string(slotNr));
		return false;
	}
	return true;
}

bool Weapon::isSpellAllowed(int slotNr, SpellID id) const {
	// check if slot with this nr exists
	if (slotNr < 0 || slotNr > m_weaponSlots.size() - 1) {
		g_logger->logError("Weapon", "This weapon has not enough slots to add this spell (slots: " + std::to_string(m_weaponSlots.size()) + ", required nr: " + std::to_string(slotNr));
		return false;
	}
	// check if this spell bean has the correct type for this. An empty spell can be added anywhere.
	if (SpellID::VOID == id) return true;
	return (m_weaponSlots.at(slotNr).spellSlot.spellType == SpellBean::getSpellBean(id).spellType);
}