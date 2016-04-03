#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "CharacterCore.h"

// Cendric in a level
class LevelMainCharacter : public virtual LevelMovableGameObject {
	friend class UserMovingBehavior;
public:
	LevelMainCharacter(Level* level);
	~LevelMainCharacter();

	void load();

	MovingBehavior* createMovingBehavior(bool asAlly = false) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly = false) override;

	void setCharacterCore(CharacterCore* core);
	void setInvisibilityLevel(int level);
	void addDamage(int damage, DamageType damageType) override;
	void addDamageOverTime(DamageOverTimeData& data) override;
	void setFeared(const sf::Time& fearedTime) override;
	void setStunned(const sf::Time& stunnedTime) override;
	// adds the item to the players inventory
	void lootItem(const std::string& item, int quantity) const;
	void lootItems(std::map<std::string, int>& items) const;
	void addGold(int gold) const;
	void removeGold(int gold) const;
	void removeItems(const std::string& item, int quantity) const;

	// ranges from 0 to 4 and helps render the main char invisibile for certain enemies / reduce the aggro range
	int getInvisibilityLevel() const;

	GameObjectType getConfiguredType() const override;

private:
	CharacterCore* m_core;
	// character core must be set when loading the weapon.
	void loadWeapon();
	void loadAnimation();
	std::map<Key, int> m_spellKeyMap;
	bool m_isQuickcast;
	int m_invisibilityLevel = 0;

	void handleAttackInput();
};