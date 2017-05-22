#pragma once

#include "global.h"
#include "Item.h"
#include "Structs/SpellModifier.h"
#include "Structs/SpellData.h"

struct SpellSlot_s final {
	SpellType spellType;
	SpellID spellID;
};

struct WeaponSlot_s final {
	SpellSlot_s spellSlot;
	std::vector<SpellModifier> spellModifiers;
};

// A weapon in cendrics / a npcs / a mobs inventory
class Weapon final : public Item {
public:
	Weapon(const std::string& itemID);

	const sf::Time& getWeaponCooldown() const;
	const sf::FloatRect& getWeaponChopRect() const;
	int getWeaponChopDamage() const;
	SpellID getCurrentSpellForSlot(int slotNr) const;
	const std::vector<SpellModifier>* getCurrentModifiersForSlot(int slotNr) const;

	// returns if the modifier has been added to the weapon spell slot.
	// it won't add the modifier if the slot is already taken if force is false
	bool addModifier(int slotNr, int modifierNr, const SpellModifier& modifier, bool force);

	// returns if the spell has been added to the weapon spell slot.
	// it won't add the spell if the slot is already taken if force is false
	bool addSpell(int slotNr, SpellID spellID, bool force);

	bool isSpellAllowed(int slotNr, SpellID spellID) const;

	const std::vector<WeaponSlot_s>& getWeaponSlots() const;

	void reload();

private:
	std::vector<WeaponSlot_s> m_weaponSlots;

	bool doesSlotExist(int slotNr) const;
};