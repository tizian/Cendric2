#pragma once

#include <map>

#include "global.h"
#include "Spell.h"

#include "Structs/SpellModifier.h"

// a class that spawns spells of a certain type - directly to the screen
class SpellCreator
{
public:
	SpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);
	virtual ~SpellCreator();

	void addModifiers(const std::vector<SpellModifier>& modifiers);

	// calculates spells using the owners attributes & the target, and executes their behaviour, adding objects to the screen.
	virtual void executeSpell(sf::Vector2f target) = 0;
	const SpellBean& getSpellBean() const;

protected:
	// filled by the subclasses
	std::vector<SpellModifierType> m_allowedModifiers;

	virtual void addDamageModifier(int level);
	virtual void addRangeModifier(int level);
	virtual void addSpeedModifier(int level);
	virtual void addCountModifier(int level);
	virtual void addReflectModifier(int level);
	virtual void addStrengthModifier(int level);
	virtual void addDurationModifier(int level);

	const AttributeBean* m_attributeBean;
	SpellBean m_spellBean;
	Level* m_level;
	Screen* m_screen;
	LevelMovableGameObject* m_owner;

	// updates the spells damage, using the mobs attributes. It adds damage and calculates critical hits
	void updateDamage(SpellBean& bean) const;
};