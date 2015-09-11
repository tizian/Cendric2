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

	// reloads depending on the core and its weapon
	void reload();

private:
	CharacterCore* m_core;
	MapInterface* m_mapInterface = nullptr;
	bool m_isVisible = false;

	void init();
	void clearAllSlots();

	Window* m_window;
	const Weapon* m_weapon;

	InventorySlot* m_weaponSlot = nullptr;
	BitmapText m_weaponName;
	BitmapText m_weaponDescription;

	std::vector<std::pair<SpellSlot, std::vector<ModifierSlot>>> m_weaponSlots;

	ModifierSlot* m_selectedModifierSlot = nullptr;

	void selectModifierSlot(ModifierSlot* selectedSlot);
	
	// used for drag & drop handling
	ModifierSlotClone* m_currentModifierClone = nullptr;
	bool m_hasDraggingStarted = false;
	bool m_isDragging = false;
	// the mouse has to move this distance while pressed to spawn a clone.
	const float DRAG_DISTANCE = 10.f;
	sf::Vector2f m_startMousePosition;
	void handleDragAndDrop();

	const int CHARACTER_SIZE = 12;
	const float TEXT_OFFSET = 20.f;
	const float MARGIN = 10.f;

	const float WIDTH = Spellbook::WIDTH;
	const float TOP = Spellbook::TOP;
	const float LEFT = Spellbook::LEFT + Spellbook::WIDTH + MARGIN;
	const float HEIGHT = Spellbook::HEIGHT;
};