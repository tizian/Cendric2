#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "SpellManager.h"
#include "InputController.h"
#include "ResourceManager.h"

// The interface overlay in a level, displaying mana + hp bars, spell cooldowns etc.
// if the keys for character screen and inventory are pressed, these will display as well.
class LevelInterface
{
public:
	LevelInterface(CharacterCore* core);

	// the spell manager of the level main character.
	void setSpellManager(SpellManager* spellManager);

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

private:
	CharacterCore* m_core;
	AttributeBean* m_attributes;
	SpellManager* m_spellManager;

	// <<<< INVENTORY >>>>
	void initInventory();
	void updateInventory();
	void renderInventory(sf::RenderTarget& target) const;
	sf::RectangleShape m_inventoryShape;
	BitmapText m_inventoryText;
	bool m_showInventory = false;
	const float INVENTORY_HEIGHT = 500.f;
	const float INVENTORY_WIDTH = 550.f;
	const float INVENTORY_TOP = 50.f;
	const float INVENTORY_LEFT = 50.f;
	const float INVENTORY_OUTLINE_THICKNESS = 5.f;

	// <<<< CHARCTER INFO >>>>
	void initCharacterInfo();
	void updateCharacterInfo();
	void renderCharacterInfo(sf::RenderTarget& target) const;
	sf::RectangleShape m_characterInfoShape;
	BitmapText m_characterInfoText;
	bool m_showCharacterInfo = false;
	const float INFO_HEIGHT = 500.f;
	const float INFO_WIDTH = 550.f;
	const float INFO_TOP = 50.f;
	const float INFO_LEFT = 50.f;
	const float INFO_OUTLINE_THICKNESS = 5.f;

	// <<<< INTERFACE BARS >>>>
	void initInterfaceBars();
	void updateInterfaceBars();
	void renderInterfaceBars(sf::RenderTarget& target) const;
	sf::RectangleShape m_hpBarOutline;
	sf::RectangleShape m_hpBar;
	sf::RectangleShape m_manaBarOutline;
	sf::RectangleShape m_manaBar;
	const float BAR_HEIGHT = 10.f;
	const float BAR_WIDTH = 200.f;
	const float BAR_OUTLINE_THICKNESS = 2.f;
	const float BAR_LEFT = 10.f;
	const float BAR_TOP = 10.f;
};