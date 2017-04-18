#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/SpellSlot.h"
#include "GUI/TexturedTabBar.h"
#include "GUI/ModifierSlot.h"
#include "GUI/GUIConstants.h"

class WeaponWindow;
class SlotClone;

// the spellbook, as displayed in a level or a map
// it takes its information about learned spells and modifiers directly from the character core
// it is only clickable when in a map.
class Spellbook {
public:
	Spellbook(CharacterCore* core, bool modifiable);
	~Spellbook();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the spellbook spells & modifiers, depending on the core
	void reload();

	static float WIDTH;
	static float SPELL_OFFSET;

private:
	CharacterCore* m_core;
	bool m_isVisible = false;
	bool m_isModifiable;

	void init();
	void clearAllSlots();

	Window* m_window = nullptr;
	WeaponWindow* m_weaponWindow = nullptr;

	BitmapText m_selectedTabText;
	std::vector<BitmapText> m_modifierTexts;

	TexturedTabBar* m_tabBar = nullptr;
	std::vector<SpellType> m_tabTypes;

	BitmapText m_emptyText;

	std::vector<ModifierSlot> m_modifierSlots;
	std::vector<std::pair<SpellSlot, std::pair<BitmapText, BitmapText>>> m_elementalSlots;
	std::vector<std::pair<SpellSlot, std::pair<BitmapText, BitmapText>>> m_twilightSlots;
	std::vector<std::pair<SpellSlot, std::pair<BitmapText, BitmapText>>> m_necromancySlots;
	std::vector<std::pair<SpellSlot, std::pair<BitmapText, BitmapText>>> m_divineSlots;

	void calculateModifierSlots();
	void calculateSpellSlots();

	SpellType m_currentTab;
	ModifierSlot* m_selectedModifierSlot = nullptr;

	void selectTab(SpellType type);
	void selectModifierSlot(ModifierSlot* selectedSlot);
	void selectSpellSlot(SpellSlot* selectedSlot);
	void equipSpell(SpellSlot* selectedSlot);
	void deselectCurrentSlot();
	std::map<SpellType, std::vector<std::pair<SpellSlot, std::pair<BitmapText, BitmapText>>>*> m_typeMap;

	// used for drag & drop handling
	SlotClone* m_currentModifierClone = nullptr;
	SlotClone* m_currentSpellClone = nullptr;
	bool m_hasDraggingStarted = false;
	bool m_isDragging = false;
	// the mouse has to move this distance while pressed to spawn a clone.
	const float DRAG_DISTANCE = 10.f;
	sf::Vector2f m_startMousePosition;
	void handleDragAndDrop();

	static const sf::Vector2f BUTTON_SIZE;
	const float MARGIN = 5.f;
};