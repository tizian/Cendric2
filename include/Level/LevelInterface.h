#pragma once

#include "global.h"
#include "SpellManager.h"

#include "GUI/GameInterface.h"
#include "GUI/HealthBar.h"
#include "GUI/BuffBar.h"
#include "GUI/QuickSlotBar.h"
#include "GUI/SpellSelection.h"

// The interface overlay in a level, displaying hp bar, spell cooldowns, buffs, quickslot bar
// if the keys for character screen and inventory are pressed, these will display as well.
class LevelInterface : public GameInterface {
public:
	LevelInterface(GameScreen* screen, LevelMainCharacter* character);
	~LevelInterface();

	// the spell manager of the level main character.
	void setSpellManager(SpellManager* spellManager);

	// used to add buffs to the buff bar
	void addBuff(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration, SpellID id = SpellID::VOID);
	// used to remove typed buffs from the buff bar
	void removeTypedBuffs(SpellID id);

	// an consumable item has been dropped. forward to quick slot bar
	void notifyConsumableDrop(const SlotClone* item);
	// consumes a consumable item
	void consumeItem(const Item& item);
	// highlight quickslots
	void highlightQuickslots(bool highlight);
	// reloads inventory and quickslot bar
	void reloadInventory(const std::string& changeditemID) override;

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	LevelMainCharacter* getMainCharacter() const;

private:
	LevelMainCharacter* m_character = nullptr;

	// <<< HEALTH BAR >>>
	HealthBar* m_healthBar = nullptr;

	// <<< SPELL SELECTION >>>
	SpellSelection* m_spellSelection = nullptr;

	// <<< QUICK SLOTS >>>
	QuickSlotBar* m_quickSlotBar = nullptr;

	// <<< BUFF BAR >>>
	BuffBar* m_buffBar = nullptr;
};