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

	void load() override;
	void onHit(Spell* spell);
	void update(const sf::Time& frameTime) override;
	void addDamage(int damage) override;
	void setDead() override;

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
	void handleInput() override;

private:
	// TODO load this map from configuration
	std::map<Key, SpellID> m_keyMap;
	CharacterCore* m_core;
	AttributeBean* m_attributes;

	// regeneration (hp, mana)
	void updateRegeneration(const sf::Time& frameTime);
	sf::Time m_timeSinceRegeneration = sf::Time::Zero;
};