#include "GUI/WeaponWindow.h"
#include "Map/MapInterface.h"
#include "GUI/SlotClone.h"

WeaponWindow::WeaponWindow(CharacterCore* core, bool clickable) {
	m_core = core;
	m_isClickable = clickable;

	init();
}

void WeaponWindow::reload() {
	m_requireReload = false;
	// reload slot and text
	delete m_weaponSlot;
	m_weapon = m_core->getWeapon();
	if (m_weapon == nullptr) {
		m_weaponSlot = new InventorySlot(g_resourceManager->getTexture(ResourceID::Texture_equipmentplaceholders), sf::Vector2i(0, 0));
		m_weaponName.setString(g_textProvider->getText("NoWeapon"));
		m_weaponDescription.setString("");
	}
	else {
		m_weaponSlot = new InventorySlot(*m_weapon, -1);
		m_weaponName.setString(g_textProvider->getText(m_weapon->getID(), "item"));
		m_weaponDescription.setString(g_textProvider->getCroppedText(m_weapon->getID(), "item_desc", GUIConstants::CHARACTER_SIZE_M,
			static_cast<int>(WIDTH - (GUIConstants::TEXT_OFFSET))));
	}
	m_weaponSlot->setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET, TOP + GUIConstants::TEXT_OFFSET));

	clearAllSlots();

	if (m_weapon == nullptr) return;

	float xOffset = LEFT + GUIConstants::TEXT_OFFSET;
	float yOffset = TOP + Spellbook::SPELL_OFFSET;
	int slotNr = 0;
	for (auto& it : m_weapon->getWeaponSlots()) {
		SpellSlot spellSlot = SpellSlot();
		if (it.spellSlot.spellID == SpellID::VOID) {
			spellSlot = SpellSlot(it.spellSlot.spellType);
		}
		else {
			spellSlot = SpellSlot(it.spellSlot.spellID);
		}
		spellSlot.setPosition(sf::Vector2f(xOffset, yOffset));
		spellSlot.setNr(slotNr);
		xOffset += SpellSlot::SIZE + 2 * MARGIN;

		std::vector<ModifierSlot> modifiers;
		for (auto& mod : it.spellModifiers) {
			modifiers.push_back(ModifierSlot(mod));
		}
		for (int i = 0; i < modifiers.size(); ++i) {
			auto& mod = modifiers[i];
			mod.setPosition(sf::Vector2f(xOffset, yOffset));
			mod.setSpellSlotNr(slotNr);
			mod.setNr(i);
			xOffset += ModifierSlot::SIZE + MARGIN;
		}

		yOffset += SpellSlot::SIZE + 2 * MARGIN;
		xOffset = LEFT + GUIConstants::TEXT_OFFSET;
		slotNr++;
		m_weaponSlots.push_back(std::pair<SpellSlot, std::vector<ModifierSlot>>({ spellSlot, modifiers }));
	}
}

void WeaponWindow::reloadSpellDesc() {
	if (m_selectedSpellSlot == nullptr) return;
	std::vector<SpellModifier> modifiers;
	for (auto& it : m_weaponSlots.at(m_selectedSpellSlot->getNr()).second) {
		modifiers.push_back(it.getModifier());
	}
	m_spellDesc->reload(m_selectedSpellSlot->getSpellID(), modifiers, &m_core->getTotalAttributes());
	m_spellDesc->show();
}

void WeaponWindow::init() {
	// init window
	delete m_window;
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	delete m_spellDesc;
	m_spellDesc = new SpellDescriptionWindow();
	m_spellDesc->setPosition(sf::Vector2f(LEFT + WIDTH + MARGIN, TOP));

	m_weaponName.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_weaponName.setColor(CENDRIC_COLOR_WHITE);
	m_weaponName.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET + MARGIN + InventorySlot::SIZE, TOP + GUIConstants::TEXT_OFFSET));

	m_weaponDescription.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_weaponDescription.setColor(CENDRIC_COLOR_LIGHT_GREY);
	m_weaponDescription.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET + MARGIN + InventorySlot::SIZE, TOP + GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + 4.f));

	reload();
}

WeaponWindow::~WeaponWindow() {
	clearAllSlots();
	delete m_window;
	delete m_currentModifierClone;
	delete m_currentSpellClone;
	delete m_weaponSlot;
	delete m_spellDesc;
}

void WeaponWindow::clearAllSlots() {
	m_weaponSlots.clear();
	m_selectedModifierSlot = nullptr;
	m_selectedSpellSlot = nullptr;
	m_spellDesc->hide();
}

void WeaponWindow::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	if (m_requireReload) reload();

	if (!m_isClickable) return;

	for (auto& it : m_weaponSlots) {
		it.first.update(frameTime);
		if (it.first.isClicked()) {
			selectSpellSlot(&it.first);
			return;
		}
		else if (it.first.isRightClicked()) {
			m_core->removeSpell(it.first.getNr());
			m_requireReload = true;
			return;
		}
		for (auto& it2 : it.second) {
			it2.update(frameTime);
			if (it2.isClicked()) {
				selectModifierSlot(&it2);
				return;
			}
			else if (it2.isRightClicked()) {
				m_core->removeModifier(it2.getSpellSlotNr(), it2.getNr());
				m_requireReload = true;
				return;
			}
		}
	}

	handleDragAndDrop();
}

void WeaponWindow::selectModifierSlot(ModifierSlot* selectedSlot) {
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;

	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedModifierSlot) return;
	if (m_selectedSpellSlot != nullptr) {
		m_selectedSpellSlot->deselect();
		m_selectedSpellSlot = nullptr;
	}
	if (m_selectedModifierSlot != nullptr) {
		m_selectedModifierSlot->deselect();
	}
	m_selectedModifierSlot = selectedSlot;
	m_selectedModifierSlot->select();
}

void WeaponWindow::selectSpellSlot(SpellSlot* selectedSlot) {
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;

	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedSpellSlot) return;
	if (m_selectedModifierSlot != nullptr) {
		m_selectedModifierSlot->deselect();
		m_selectedModifierSlot = nullptr;
	}
	if (m_selectedSpellSlot != nullptr) {
		m_selectedSpellSlot->deselect();
	}
	m_selectedSpellSlot = selectedSlot;
	m_selectedSpellSlot->select();
	reloadSpellDesc();
}

void WeaponWindow::handleDragAndDrop() {
	if (!m_hasDraggingStarted) return;
	if (!(g_inputController->isMousePressedLeft())) {
		if (m_selectedModifierSlot != nullptr) {
			if (m_currentModifierClone != nullptr && !(m_currentModifierClone->getBoundingBox()->intersects(*m_selectedModifierSlot->getBoundingBox()))) {
				m_core->removeModifier(m_selectedModifierSlot->getSpellSlotNr(), m_selectedModifierSlot->getNr());
				m_requireReload = true;
			}
			else {
				m_selectedModifierSlot->activate();
			}
		}
		if (m_selectedSpellSlot != nullptr) {
			if (m_currentSpellClone != nullptr && !(m_currentSpellClone->getBoundingBox()->intersects(*m_selectedSpellSlot->getBoundingBox()))) {
				m_core->removeSpell(m_selectedSpellSlot->getNr());
				m_requireReload = true;
			}
			else {
				m_selectedSpellSlot->activate();
			}
		}
		delete m_currentModifierClone;
		delete m_currentSpellClone;
		m_currentModifierClone = nullptr;
		m_currentSpellClone = nullptr;
		m_hasDraggingStarted = false;
		m_isDragging = false;
		return;
	}
	sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
	if (!m_isDragging) {
		if (DRAG_DISTANCE < std::sqrt(
			(mousePos.x - m_startMousePosition.x) * (mousePos.x - m_startMousePosition.x) +
			(mousePos.y - m_startMousePosition.y) * (mousePos.y - m_startMousePosition.y))) {
			m_isDragging = true;
			delete m_currentModifierClone;
			delete m_currentSpellClone;
			if (m_selectedModifierSlot != nullptr) {
				m_currentModifierClone = new SlotClone(m_selectedModifierSlot);
				m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIZE / 2.f, InventorySlot::SIZE / 2.f));
				m_selectedModifierSlot->deactivate();
			}
			else if (m_selectedSpellSlot != nullptr) {
				m_currentSpellClone = new SlotClone(m_selectedSpellSlot);
				m_currentSpellClone->setPosition(mousePos);
				m_selectedSpellSlot->deactivate();
			}
		}
	}
	else {
		if (m_currentModifierClone != nullptr)
			m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIZE / 2.f, InventorySlot::SIZE / 2.f));
		if (m_currentSpellClone != nullptr)
			m_currentSpellClone->setPosition(mousePos - sf::Vector2f(SpellSlot::SIZE / 2.f, SpellSlot::SIZE / 2.f));
	}
}

bool WeaponWindow::isVisible() const {
	return m_isVisible;
}

void WeaponWindow::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	m_spellDesc->render(target);
	target.draw(m_weaponName);
	target.draw(m_weaponDescription);
	m_weaponSlot->render(target);

	for (auto& it : m_weaponSlots) {
		it.first.render(target);
		for (auto& it2 : it.second) {
			it2.render(target);
		}
	}

	if (m_currentModifierClone != nullptr) {
		m_currentModifierClone->render(target);
	}
	if (m_currentSpellClone != nullptr) {
		m_currentSpellClone->render(target);
	}

	for (auto& it : m_weaponSlots) {
		for (auto& it2 : it.second) {
			it2.renderAfterForeground(target);
		}
	}
}

void WeaponWindow::highlightSpellSlots(SpellType type, bool highlight) {
	for (auto& it : m_weaponSlots) {
		if (!highlight || it.first.getSpellType() == type) {
			if (highlight) {
				it.first.highlight();
			}
			else {
				it.first.unhighlight();
			}
		}
	}
}

void WeaponWindow::highlightModifierSlots(SpellModifierType type, bool highlight) {
	for (auto& it : m_weaponSlots) {
		std::vector<SpellModifierType> allowedMods = SpellData::getAllowedModifiers(it.first.getSpellID());
		if (!highlight || std::find(allowedMods.begin(), allowedMods.end(), type) != allowedMods.end()) {
			for (auto& it2 : it.second) {
				if (highlight) {
					it2.highlight();
				}
				else {
					it2.unhighlight();
				}
			}
		}
	}
}

void WeaponWindow::notifyModifierDrop(SlotClone* clone) {
	if (clone == nullptr) return;
	const ModifierSlot *ms = static_cast<const ModifierSlot *>(clone->getOriginalSlot());
	SpellModifier modifier = ms->getModifier();

	for (auto& it : m_weaponSlots) {
		std::vector<SpellModifierType> allowedMods = SpellData::getAllowedModifiers(it.first.getSpellID());
		bool isModifierAllowed = std::find(allowedMods.begin(), allowedMods.end(), modifier.type) != allowedMods.end();
		if (!isModifierAllowed) continue;
		for (auto& modifierSlot : it.second) {
			if (clone->getBoundingBox()->intersects(*modifierSlot.getBoundingBox())) {
				m_core->addModifier(ms->getModifier(), modifierSlot.getSpellSlotNr(), modifierSlot.getNr());
				m_requireReload = true;
				break;
			}
		}
	}
}

void WeaponWindow::notifySpellDrop(SlotClone* clone) {
	if (clone == nullptr) return;
	const SpellSlot *ss = static_cast<const SpellSlot *>(clone->getOriginalSlot());
	SpellType type = ss->getSpellType();
	for (auto& it : m_weaponSlots) {
		if (type == it.first.getSpellType() && clone->getBoundingBox()->intersects(*it.first.getBoundingBox())) {
			m_core->addSpell(ss->getSpellID(), it.first.getNr());
			m_requireReload = true;
			break;
		}
	}
}

void WeaponWindow::show() {
	reload();
	m_isVisible = true;
}

void WeaponWindow::hide() {
	m_isVisible = false;
	delete m_currentModifierClone;
	m_currentModifierClone = nullptr;
	delete m_currentSpellClone;
	m_currentSpellClone = nullptr;
	m_isDragging = false;
	m_hasDraggingStarted = false;
	m_spellDesc->hide();
}