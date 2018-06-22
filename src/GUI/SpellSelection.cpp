#include "GUI/SpellSelection.h"
#include "Controller/GamepadMappings/GamepadMappings.h"

SpellSelection::SpellSelection(SpellManager* manager) {
	m_spellManager = manager;
	m_spellManager->setSpellSelection(this);
	reload();
	selectSlot(m_spellManager->getSelectedSpellID());
}

SpellSelection::~SpellSelection() {
	CLEAR_VECTOR(m_spellSlots);
}

void SpellSelection::activateSlot(int spellNr, const sf::Time& cooldown) {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellSlots.size()) - 1) return;
	if (m_spellSlots[m_selectedSlot]->getSpellID() == SpellID::VOID) return;
	m_spellSlots[spellNr]->playAnimation(cooldown);
}

void SpellSelection::selectSlot(int spellNr) {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellSlots.size()) - 1) return;
	if (m_spellSlots[m_selectedSlot]->getSpellID() == SpellID::VOID) return;
	m_spellSlots[m_selectedSlot]->deselect();
	m_spellSlots[spellNr]->select();
	m_selectedSlot = spellNr;

	reloadGamepadTexts();
}

void SpellSelection::update(const sf::Time& frametime) {
	for (int i = 0; i < static_cast<int>(m_spellSlots.size()); i++) {
		m_spellSlots[i]->update(frametime);
		if (m_spellSlots[i]->isClicked() && !m_spellSlots[i]->isLocked()) {
			m_spellManager->getOwner()->getScreen()->getCharacterCore()->setWeaponSpell(m_spellSlots[i]->getKey());
			m_spellManager->setAndExecuteSpell(i);
		}
	}
}

void SpellSelection::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	for (auto it : m_spellSlots) {
		it->render(target);
	}
}

void SpellSelection::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	for (auto it : m_spellSlots) {
		it->renderAfterForeground(target);
	}
}

void SpellSelection::reload() {
	CLEAR_VECTOR(m_spellSlots);

	auto& lockedMagic = m_spellManager->getOwner()->getLevel()->getLockedMagic();
	float offset = SpellSlot::ICON_OFFSET;
	for (auto& it : m_spellManager->getSpellMap()) {
		SpellSlot* slot = new SpellSlot(it->getSpellData());
		slot->setPosition(sf::Vector2f(
			SPELLSELECTION_OFFSET.x + offset,
			WINDOW_HEIGHT - (SpellSlot::ICON_SIZE + SPELLSELECTION_OFFSET.y + SpellSlot::ICON_OFFSET)));
		
		slot->setLocked(contains(lockedMagic, slot->getSpellType()));
		slot->playAnimation(m_spellManager->getCooldown(it->getSpellData().id));

		m_spellSlots.push_back(slot);

		offset += (SPELLSLOT_SPACING + SpellSlot::SIZE);
	}
	m_selectedSlot = 0;
	if (!m_spellSlots.empty()) {
		selectSlot(0);
	}

	reloadGamepadTexts();
}

void SpellSelection::reloadGamepadTexts() {
	if (!g_inputController->isGamepadConnected()) {
		for (auto slot : m_spellSlots) {
			slot->setKeyboardInputText();
		}

		return;
	}

	for (auto i = 0; i < static_cast<int>(m_spellSlots.size()); ++i) {
		if (i == m_selectedSlot - 1) {
			m_spellSlots[i]->setInputKeyText(GamepadMappings::getKeyName(Key::PreviousSpell));
		}
		else if (i == m_selectedSlot + 1) {
			m_spellSlots[i]->setInputKeyText(GamepadMappings::getKeyName(Key::NextSpell));
		}
		else if (i == m_selectedSlot) {
			m_spellSlots[i]->setInputKeyText(GamepadMappings::getKeyName(Key::Attack));
		}
		else {
			m_spellSlots[i]->setInputKeyText("");
		}
	}
}


bool SpellSelection::isSlotLocked(int spellNr) const {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellSlots.size())) {
		return false;
	}
	return m_spellSlots.at(spellNr)->isLocked();
}

std::vector<SpellSlot*>& SpellSelection::getSlots() {
	return m_spellSlots;
}

void SpellSelection::show() {
	m_isVisible = true;
}

void SpellSelection::hide() {
	m_isVisible = false;
}