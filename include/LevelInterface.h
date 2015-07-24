#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "SpellManager.h"
#include "InputController.h"
#include "ResourceManager.h"

#include "GUI/HealthBar.h"
#include "GUI/SpellSelection.h"
#include "GUI/Inventory.h"
#include "GUI/CharacterInfo.h"

// The interface overlay in a level, displaying hp bar, spell cooldowns etc.
// if the keys for character screen and inventory are pressed, these will display as well.
class LevelInterface
{
public:
	LevelInterface(CharacterCore* core, LevelMainCharacter* character);

	// the spell manager of the level main character.
	void setSpellManager(SpellManager* spellManager);

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

private:
	// <<< INVENTORY >>>
	Inventory m_inventory;
	bool m_showInventory = false;
	void updateInventory();

	// <<<< CHARCTER INFO >>>>
	CharacterInfo m_characterInfo;
	bool m_showCharacterInfo = false;
	void updateCharacterInfo();
	
	// <<<< HEALTH BAR >>>>
	HealthBar m_healthBar;
};