#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "Controller/InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/SpellSlot.h"
#include "GUI/TexturedTabBar.h"
#include "GUI/ModifierSlot.h"
#include "GUI/GUIConstants.h"
#include "GUI/SelectableWindow.h"
#include "GUI/SpellButtonGroup.h"

class WeaponWindow;
class SlotClone;
class WorldInterface;

// the spellbook, as displayed in a level or a map
// it takes its information about learned spells and modifiers directly from the character core
// it is only clickable when in a map.
class Spellbook final : public SelectableWindow {
public:
	Spellbook(WorldInterface* _interface, bool modifiable);
	~Spellbook();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void reload();

	WeaponWindow* getWeaponWindow() const;
	bool isGamepadSelection() const;
	void stopGamepadSelection();
	void notifyGamepadSelected();

	static float WIDTH;
	static float SPELL_OFFSET;

protected:
	void updateWindowSelected() override;
	void reloadButtonGroup();
	void updateButtonActions();
	void startGamepadSelection();

private:
	CharacterCore* m_core;
	WorldInterface* m_interface;
	bool m_isVisible = false;
	bool m_isModifiable;

	void init();
	void clearAllSlots();

	WeaponWindow* m_weaponWindow = nullptr;

	BitmapText m_selectedTabText;
	std::vector<BitmapText> m_modifierTexts;

	TexturedTabBar* m_tabBar = nullptr;
	std::vector<SpellType> m_tabTypes;

	BitmapText m_emptyText;

	std::vector<ModifierSlot*> m_modifierSlots;
	std::vector<std::pair<SpellSlot*, std::pair<BitmapText, BitmapText>>> m_elementalSlots;
	std::vector<std::pair<SpellSlot*, std::pair<BitmapText, BitmapText>>> m_twilightSlots;
	std::vector<std::pair<SpellSlot*, std::pair<BitmapText, BitmapText>>> m_necromancySlots;
	std::vector<std::pair<SpellSlot*, std::pair<BitmapText, BitmapText>>> m_divineSlots;

	SpellButtonGroup* m_buttonGroup = nullptr;

	void calculateModifierSlots();
	void calculateSpellSlots();

	SpellType m_currentTab;
	ModifierSlot* m_selectedModifierSlot = nullptr;

	void selectTab(SpellType type);
	void selectModifierSlot(ModifierSlot* selectedSlot);
	void selectSpellSlot(SpellSlot* selectedSlot);
	void equipSpell(SpellSlot* selectedSlot);
	void deselectCurrentSlot();
	std::map<SpellType, std::vector<std::pair<SpellSlot*, std::pair<BitmapText, BitmapText>>>*> m_typeMap;

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
	bool m_isGamepadSelection = false;

	static const sf::Vector2f BUTTON_SIZE;
	const float MARGIN = 5.f;
};