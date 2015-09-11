#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/SpellSlot.h"
#include "GUI/ModifierSlot.h"
#include "GUI/ModifierSlotClone.h"
#include "GUI/InventorySlot.h"
#include "GUI/Spellbook.h"
#include "Weapon.h"

class MapInterface;

// weapon screen, describing a weapon and its equipped spells
// is displayed and updated as a part of the Spellbook.
class WeaponWindow
{
public:
	WeaponWindow(MapInterface* _interface);
	~WeaponWindow();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// highlights all slots that could take a spell modifier of this type.
	// if highlight is false, unhighlights all modifier slots.
	void highlightModifierSlots(SpellModifierType type, bool highlight);
	void notifyModifierDrop(ModifierSlotClone* clone);

	// highlights all slots that could take a spell of this type.
	// if highlight is false, unhighlights all spell slots.
	void highlightSpellSlots(SpellType type, bool highlight);
	void notifySpellDrop(SpellSlotClone* clone);

	// reloads depending on the core and its weapon
	void reload();

private:
	CharacterCore* m_core;
	MapInterface* m_mapInterface = nullptr;
	bool m_isVisible = false;
	bool m_requireReload = false;

	void init();
	void clearAllSlots();

	Window* m_window;
	const Weapon* m_weapon;

	InventorySlot* m_weaponSlot = nullptr;
	BitmapText m_weaponName;
	BitmapText m_weaponDescription;

	std::vector<std::pair<SpellSlot, std::vector<ModifierSlot>>> m_weaponSlots;

	void selectModifierSlot(ModifierSlot* selectedSlot);
	void selectSpellSlot(SpellSlot* selectedSlot);

	ModifierSlot* m_selectedModifierSlot = nullptr;
	SpellSlot* m_selectedSpellSlot = nullptr;

	// used for drag & drop handling
	ModifierSlotClone* m_currentModifierClone = nullptr;
	SpellSlotClone* m_currentSpellClone = nullptr;
	bool m_hasDraggingStarted = false;
	bool m_isDragging = false;
	// the mouse has to move this distance while pressed to spawn a clone.
	const float DRAG_DISTANCE = 10.f;
	sf::Vector2f m_startMousePosition;
	void handleDragAndDrop();

	const int CHARACTER_SIZE = 12;
	const float TEXT_OFFSET = 20.f;
	const float MARGIN = 10.f;

	const float TOP = Spellbook::TOP;
	const float LEFT = Spellbook::LEFT + Spellbook::WIDTH + MARGIN;
	const float HEIGHT = Spellbook::HEIGHT;
	const float WIDTH = 2 * TEXT_OFFSET + 2 * SpellSlot::RADIUS + 4 * MARGIN + 3 * ModifierSlot::SIDE_LENGTH;
};