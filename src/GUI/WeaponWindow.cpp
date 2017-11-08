#include "GUI/WeaponWindow.h"
#include "GUI/SlotClone.h"
#include "Level/LevelInterface.h"
#include "ResourceManager.h"

WeaponWindow::WeaponWindow(WorldInterface* _interface, bool modifiable) {
	m_interface = _interface;
	m_core = m_interface->getCore();
	m_isModifiable = modifiable;

	init();
}

void WeaponWindow::addCloseButton(const std::function<void()>& agent) {
	m_window->addCloseButton(agent);
}


void WeaponWindow::notifyLevelReload() {
	auto levelInterface = dynamic_cast<LevelInterface*>(m_interface);
	if (!levelInterface) return;
	levelInterface->notifyReloadWeapon();
}

void WeaponWindow::reload() {
	m_requireReload = false;
	// reload slot and text
	delete m_weaponSlot;
	m_weapon = m_core->getWeapon();
	if (m_weapon == nullptr) {
		m_weaponSlot = new InventorySlot(g_resourceManager->getTexture(GlobalResource::TEX_EQUIPMENTPLACEHOLDERS), sf::Vector2i(0, 0), ItemType::Equipment_weapon);
		m_weaponName.setString(g_textProvider->getText("NoWeapon"));
	}
	else {
		m_weaponSlot = new InventorySlot(m_weapon->getID(), -1);
		m_weaponName.setString(g_textProvider->getCroppedText(m_weapon->getID(), "item", GUIConstants::CHARACTER_SIZE_M,
			static_cast<int>(WIDTH - (2 * GUIConstants::TEXT_OFFSET + 2.f * MARGIN + InventorySlot::SIZE))));
	}
	m_weaponSlot->setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET + InventorySlot::ICON_OFFSET, TOP + GUIConstants::TEXT_OFFSET + InventorySlot::ICON_OFFSET));
	m_weaponName.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET + 2.f * MARGIN + InventorySlot::SIZE, TOP + GUIConstants::TEXT_OFFSET + 0.5f * (InventorySlot::SIZE - m_weaponName.getBounds().height)));

	clearAllSlots();

	if (m_weapon == nullptr) {
		notifyLevelReload();
		return;
	}

	float xOffset = LEFT + GUIConstants::TEXT_OFFSET / 2.f + SpellSlot::ICON_OFFSET;
	float yOffset = TOP + Spellbook::SPELL_OFFSET;
	int slotNr = 0;
	if (m_weapon->getWeaponSlots().empty()) {
		m_noSlotsText.setString(g_textProvider->getCroppedText("EquipWeapon", GUIConstants::CHARACTER_SIZE_M,
			static_cast<int>(WIDTH - (GUIConstants::TEXT_OFFSET + SpellSlot::ICON_OFFSET))));
		return;
	}
	m_noSlotsText.setString("");
	for (auto& it : m_weapon->getWeaponSlots()) {
		SpellSlot spellSlot;
		if (it.spellSlot.spellID == SpellID::VOID) {
			spellSlot = SpellSlot(it.spellSlot.spellType);
		}
		else {
			spellSlot = SpellSlot(it.spellSlot.spellID, it.spellSlot.spellType == SpellType::Meta);
		}
		spellSlot.setPosition(sf::Vector2f(xOffset, yOffset));
		spellSlot.setNr(slotNr);
		xOffset += SpellSlot::SIZE + MARGIN;

		std::vector<ModifierSlot> modifiers;
		for (auto& mod : it.spellModifiers) {
			modifiers.push_back(ModifierSlot(mod));
		}
		for (size_t i = 0; i < modifiers.size(); ++i) {
			auto& mod = modifiers[i];
			mod.setPosition(sf::Vector2f(xOffset, yOffset));
			mod.setSpellSlotNr(slotNr);
			mod.setNr(static_cast<int>(i));
			xOffset += ModifierSlot::SIZE + MARGIN;
		}

		yOffset += SpellSlot::SIZE + MARGIN;
		xOffset = LEFT + GUIConstants::TEXT_OFFSET / 2.f + SpellSlot::ICON_OFFSET;
		slotNr++;
		m_weaponSlots.push_back(std::pair<SpellSlot, std::vector<ModifierSlot>>({ spellSlot, modifiers }));
	}
	if (m_selectedSpellSlot != nullptr && m_selectedSpellSlot->getNr() != -1) {
		m_weaponSlots.at(m_selectedSpellSlot->getNr()).first.select();
	}

	notifyLevelReload();
}

void WeaponWindow::reloadSpellDesc() {
	if (m_selectedSpellSlot == nullptr) return;
	std::vector<SpellModifier> modifiers;
	if (m_selectedSpellSlot->getNr() != -1) {
		for (auto& it : m_weaponSlots.at(m_selectedSpellSlot->getNr()).second) {
			modifiers.push_back(it.getModifier());
		}
	}
	m_spellDesc->reload(m_selectedSpellSlot->getSpellID(), modifiers, m_core->getTotalAttributes());
	m_spellDesc->show();
}

void WeaponWindow::init() {
	// init window
	delete m_window;
	sf::FloatRect box(LEFT, TOP, WIDTH, GUIConstants::GUI_WINDOW_HEIGHT);
	m_window = new Window(box,
		GUIOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	delete m_spellDesc;
	m_spellDesc = new SpellDescriptionWindow();
	m_spellDesc->setPosition(sf::Vector2f(LEFT + WIDTH + MARGIN, TOP));

	m_weaponName.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_weaponName.setColor(COLOR_WHITE);

	m_noSlotsText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_noSlotsText.setColor(COLOR_WHITE);
	m_noSlotsText.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET / 2.f + SpellSlot::ICON_OFFSET, TOP + Spellbook::SPELL_OFFSET));

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
	if (m_selectedSpellSlot == nullptr) {
		m_spellDesc->hide();
	}
}

void WeaponWindow::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	if (m_requireReload) reload();

	for (auto& it : m_weaponSlots) {
		it.first.update(frameTime);
		if (it.first.isMousedOver() && !m_hasDraggingStarted) {
			selectSpellSlot(&it.first);
			if (m_isModifiable && it.first.isDoubleClicked()) {
				m_core->removeSpell(it.first.getNr());
				m_requireReload = true;
				return;
			}
			if (m_isModifiable && it.first.isRightClicked()) {
				m_core->removeSpell(it.first.getNr());
				m_requireReload = true;
				return;
			}
		}
		
		for (auto& it2 : it.second) {
			it2.update(frameTime);
			if (it2.isClicked()) {
				selectModifierSlot(&it2);
				if (m_isModifiable && it2.isDoubleClicked()) {
					m_core->removeModifier(it2.getSpellSlotNr(), it2.getNr());
					m_requireReload = true;
				}
				return;
			}
			else if (m_isModifiable && it2.isRightClicked()) {
				m_core->removeModifier(it2.getSpellSlotNr(), it2.getNr());
				m_requireReload = true;
				return;
			}
		}
	}

	if (m_isModifiable) {
		handleDragAndDrop();
	}

	m_window->update(frameTime);
}

void WeaponWindow::selectModifierSlot(ModifierSlot* selectedSlot) {
	if (selectedSlot == nullptr) return;

	if (g_inputController->isMouseJustPressedLeftRaw()) {
		m_hasDraggingStarted = true;
		m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	}
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
	if (selectedSlot->isEmpty()) return;

	if (g_inputController->isMouseJustPressedLeftRaw()) {
		m_hasDraggingStarted = true;
		m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	}
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
			if (m_currentModifierClone != nullptr && !fastIntersect(*m_currentModifierClone->getBoundingBox(), *m_selectedModifierSlot->getBoundingBox())) {
				m_core->removeModifier(m_selectedModifierSlot->getSpellSlotNr(), m_selectedModifierSlot->getNr());
				m_requireReload = true;
			}
			else {
				m_selectedModifierSlot->activate();
			}
		}
		if (m_selectedSpellSlot != nullptr) {
			if (m_currentSpellClone != nullptr && !fastIntersect(*m_currentSpellClone->getBoundingBox(), *m_selectedSpellSlot->getBoundingBox())) {
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
				m_currentSpellClone->setPosition(mousePos - sf::Vector2f(SpellSlot::SIZE / 2.f, SpellSlot::SIZE / 2.f));
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
	target.draw(m_noSlotsText);
	m_weaponSlot->render(target);

	for (auto& it : m_weaponSlots) {
		it.first.render(target);
		it.first.renderAfterForeground(target);
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
		if (!highlight || it.first.getSpellType() == SpellType::Meta || it.first.getSpellType() == type) {
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
		if (!highlight || contains(allowedMods, type)) {
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
	const ModifierSlot* ms = static_cast<const ModifierSlot*>(clone->getOriginalSlot());
	SpellModifier modifier = ms->getModifier();

	for (auto& it : m_weaponSlots) {
		std::vector<SpellModifierType> allowedMods = SpellData::getAllowedModifiers(it.first.getSpellID());
		if (!contains(allowedMods, modifier.type)) continue;
		for (auto& modifierSlot : it.second) {
			if (fastIntersect(*clone->getBoundingBox(), *modifierSlot.getBoundingBox())) {
				m_core->addModifier(ms->getModifier(), modifierSlot.getSpellSlotNr(), modifierSlot.getNr());
				m_requireReload = true;
				break;
			}
		}
	}
}

void WeaponWindow::notifySpellDrop(SlotClone* clone) {
	if (clone == nullptr) return;
	const SpellSlot* ss = static_cast<const SpellSlot*>(clone->getOriginalSlot());
	SpellType type = ss->getSpellType();
	for (auto& slot : m_weaponSlots) {
		if ((slot.first.getSpellType() == SpellType::Meta || type == slot.first.getSpellType()) 
			&& fastIntersect(*clone->getBoundingBox(), *slot.first.getBoundingBox())) 
		{
			m_core->addSpell(ss->getSpellID(), slot.first.getNr());
			m_requireReload = true;
			break;
		}
	}
}

void WeaponWindow::equipSpell(const SpellSlot* spellSlot) {
	if (spellSlot == nullptr) return;

	bool isEquipped = false;
	SpellType type = spellSlot->getSpellType();
	for (auto& slot : m_weaponSlots) {
		if (slot.first.getSpellID() == spellSlot->getSpellID()) {
			// spell already equipped.
			return;
		}
	}
	for (auto& slot : m_weaponSlots) {
		if ((slot.first.getSpellType() == SpellType::Meta || type == slot.first.getSpellType())
			&& slot.first.isEmpty()) {
			m_core->addSpell(spellSlot->getSpellID(), slot.first.getNr());
			isEquipped = true;
			break;
		}
	}
	if (!isEquipped) {
		for (auto& slot : m_weaponSlots) {
			if (slot.first.getSpellType() == SpellType::Meta || type == slot.first.getSpellType()) {
				m_core->addSpell(spellSlot->getSpellID(), slot.first.getNr());
				break;
			}
		}
	}
	m_requireReload = true;
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