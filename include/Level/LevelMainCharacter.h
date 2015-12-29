#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "SpellManager.h"
#include "Screen.h"
#include "CharacterCore.h"

// Cendric in a level
class LevelMainCharacter : public LevelMovableGameObject {
public:
	LevelMainCharacter(Level* level);
	~LevelMainCharacter();

	void load();

	void setCharacterCore(CharacterCore* core);
	void setInvisibilityLevel(int level);
	void addDamage(int damage, DamageType damageType) override;
	void addDamageOverTime(const DamageOverTimeData& data) override;
	// adds the item to the players inventory
	void lootItem(const std::string& item, int quantity) const;
	void lootItems(std::map<std::string, int>& items) const;
	void addGold(int gold) const;
	void removeGold(int gold) const;
	void removeItems(const std::string& item, int quantity) const;

	// ranges from 0 to 4 and helps render the main char invisibile for certain enemies / reduce the aggro range
	int getInvisibilityLevel() const;
	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	sf::Time getConfiguredFightAnimationTime() const override;
	GameObjectType getConfiguredType() const override;

protected:
	float getConfiguredDampingGroundPersS() const;
	// handle input and calculate the next position
	void handleMovementInput() override;
	void handleAttackInput() override;

private:
	CharacterCore* m_core;
	// character core must be set when loading the weapon.
	void loadWeapon();
	std::map<Key, int> m_spellKeyMap;
	bool m_isQuickcast;
	int m_invisibilityLevel = 0;
};