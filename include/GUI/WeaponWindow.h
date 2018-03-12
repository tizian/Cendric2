#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/SpellSlot.h"
#include "GUI/ModifierSlot.h"
#include "GUI/InventorySlot.h"
#include "GUI/Spellbook.h"
#include "GUI/SpellDescriptionWindow.h"
#include "World/Weapon.h"
#include "GUI/GUIConstants.h"

class SlotClone;
class WorldInterface;

// weapon screen, describing a weapon and its equipped spells
// is displayed and updated as a part of the Spellbook.
// is only modifiable when modifiable is true
class WeaponWindow final {
	friend class Spellbook;
public:
	WeaponWindow(WorldInterface* _interface, bool modifiable);
	~WeaponWindow();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void addCloseButton(const std::function<void()>& agent);

	// highlights all slots that could take a spell modifier of this type.
	// if highlight is false, unhighlights all modifier slots.
	void highlightModifierSlots(SpellModifierType type, bool highlight);
	void notifyModifierDrop(SlotClone* clone);

	// highlights all slots that could take a spell of this type.
	// if highlight is false, unhighlights all spell slots.
	void highlightSpellSlots(SpellType type, bool highlight);
	void notifySpellDrop(SlotClone* clone);
	void equipSpell(const SpellSlot* spellSlot);

	// reloads depending on the core and its weapon
	void reload();

private:
	WorldInterface* m_interface;
	CharacterCore* m_core;
	SpellDescriptionWindow* m_spellDesc = nullptr;
	void reloadSpellDesc();
	bool m_isVisible = false;
	bool m_requireReload = false;
	bool m_isModifiable;

	void init();
	void clearAllSlots();
	void notifyLevelReload();

	Window* m_window = nullptr;
	const Weapon* m_weapon;

	InventorySlot* m_weaponSlot = nullptr;
	BitmapText m_weaponName;
	BitmapText m_noSlotsText;

	std::vector<std::pair<SpellSlot*, std::vector<ModifierSlot*>>> m_weaponSlots;

	void selectModifierSlot(ModifierSlot* selectedSlot);
	void selectSpellSlot(SpellSlot* selectedSlot);

	void removeModifier(const ModifierSlot& slot);
	void removeSpell(const SpellSlot& slot);

	ModifierSlot* m_selectedModifierSlot = nullptr;
	SpellSlot* m_selectedSpellSlot = nullptr;
	int m_previouslySelectedSpellSlot = -1;

	// used for drag & drop handling
	SlotClone* m_currentModifierClone = nullptr;
	SlotClone* m_currentSpellClone = nullptr;
	bool m_hasDraggingStarted = false;
	bool m_isDragging = false;
	// the mouse has to move this distance while pressed to spawn a clone.
	const float DRAG_DISTANCE = 10.f;
	sf::Vector2f m_startMousePosition;
	void handleDragAndDrop();
	void stopDragging();

	const float MARGIN = 5.f; 

	const float TOP = GUIConstants::TOP;
	const float LEFT = GUIConstants::LEFT + Spellbook::WIDTH + MARGIN;
	const float WIDTH = 2 * GUIConstants::TEXT_OFFSET + SpellSlot::SIZE + 4 * MARGIN + 3 * ModifierSlot::SIZE;
};