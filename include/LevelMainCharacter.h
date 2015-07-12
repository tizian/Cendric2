#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "SpellManager.h"
#include "Screen.h"
#include "CharacterCore.h"

// Cendric in a level
class LevelMainCharacter : public LevelMovableGameObject
{
public:
	LevelMainCharacter(Level* level);
	~LevelMainCharacter();

	void load();

	void setCharacterCore(CharacterCore* core);
	// adds the item to the players inventory
	void lootItem(ItemID item, int quantity) const;
	void lootItems(std::map<ItemID, int>& items) const;
	void addGold(int gold) const;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;

protected:
	float getConfiguredDampingGroundPersS() const;
	// handle input and calculate the next position
	void handleMovementInput() override;
	void handleAttackInput() override;

private:
	// TODO: load this from configuration
	std::map<Key, SpellID> m_keyMap;
	CharacterCore* m_core;
};