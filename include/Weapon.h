#pragma once

#include "global.h"
#include "Item.h"
#include "Structs/SpellModifier.h"
#include "Structs/SpellBean.h"

// A weapon in cendrics / a npcs / a mobs inventory
class Weapon : public Item
{
public:
	Weapon(const ItemBean& bean);
	
	const sf::Time& getWeaponCooldown() const;
	const sf::FloatRect& getWeaponChopRect() const;
	int getWeaponChopDamage() const;
	SpellID getCurrentSpellForSlot(int slotNr) const;
	const std::map<SpellModifierType, SpellModifier>* getCurrentModifiersForSlot(int slotNr) const;
	
	// returns if the modifier has been added to the weapon spell slot.
	// it won't add the modifier if the slot is already taken if force is false
	bool addModifier(int slotNr, const SpellModifier& modifier, bool force);

	// returns if the spell has been added to the weapon spell slot.
	// it won't add the spell if the slot is already taken if force is false
	bool addSpell(int slotNr, SpellID spellID, bool force);

private:

	std::vector<std::pair<
		std::pair<SpellType, SpellID>, 
		std::pair<
			int,
			std::map<SpellModifierType, SpellModifier>>>> m_weaponSlots;

	bool doesSlotExist(int slotNr) const;
	bool isSpellAllowed(int slotNr, SpellID spellID) const;
};