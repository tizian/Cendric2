#pragma once

#include "global.h"
#include "Spells/SpellManager.h"

#include "GUI/WorldInterface.h"
#include "GUI/HealthBar.h"
#include "GUI/BuffBar.h"
#include "GUI/SpellSelection.h"

// The interface overlay in a level, displaying hp bar, spell cooldowns, buffs, quickslot bar
// if the keys for character screen and inventory are pressed, these will display as well.
class LevelInterface final : public WorldInterface {
public:
	LevelInterface(WorldScreen* screen, LevelMainCharacter* character);
	~LevelInterface();

	// the spell manager of the level main character.
	void setSpellManager(SpellManager* spellManager);
	// the permanent core, set be the level screen
	void setPermanentCore(CharacterCore* permanentCore);
	// returns the buff bar
	BuffBar& getBuffBar();
	// consumes a consumable item
	void consumeItem(const std::string& itemID);
	// clears the consumabed food vector
	void clearConsumedFood();
	// restores the consumed food
	void restoreConsumedFood();
	// reloads inventory and quickslot bar
	void reloadInventory(const std::string& changeditemID) override;
	// notifies the character info
	void notifyCharacterInfo();
	// notifies that attributes have to be reloaded
	void notifyReloadAttributes();
	// notifies that the equipment has to be reloaded
	void notifyReloadEquipment();
	// notifies that the weapon has to be reloaded
	void notifyReloadWeapon();
	// returns if an enemy health bar is currently displayed
	bool isEnemyHealthBarDisplayed();
	// sets the whose healthbar should be displayed
	void setEnemyForHealthBar(const Enemy* enemy);
	// wether this is a boss level
	bool isBossLevel() const;

	void render(sf::RenderTarget& target) override;
	void renderAfterForeground(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	LevelMainCharacter* getMainCharacter() const;
	// the permanent core from the level screen
	// used to write permanent states back.
	CharacterCore* getPermanentCore() const;

private:
	LevelMainCharacter* m_character = nullptr;
	CharacterCore* m_permanentCore = nullptr;

	// <<< HEALTH BARS >>>
	HealthBar* m_mainCharHealthBar = nullptr;
	HealthBar* m_enemyHealthBar = nullptr;	// TODO: what if we have multiple bosses?

	// <<< SPELL SELECTION >>>
	SpellSelection* m_spellSelection = nullptr;

	// <<< BUFF BAR >>>
	BuffBar* m_buffBar = nullptr;

	// food items consumed in this run
	std::map<std::string, int> m_consumedFood;
};