#include "GUI/Spellbook.h"
#include "Map/MapInterface.h"
#include "GUI/WeaponWindow.h"
#include "GUI/SlotClone.h"

float Spellbook::WIDTH = (WINDOW_WIDTH - GUIConstants::LEFT - 20.f) / 3.f;
float Spellbook::HEIGHT = WINDOW_HEIGHT - 150.f;
float Spellbook::SPELL_OFFSET = 115.f;
const sf::Vector2f Spellbook::BUTTON_SIZE = sf::Vector2f(70.f, 57.f);

Spellbook::Spellbook(CharacterCore* core, bool modifiable) {
	m_core = core;
	m_isModifiable = modifiable;
	 
	init();
}

void Spellbook::init() {
	// init window
	sf::FloatRect box(GUIConstants::LEFT, GUIConstants::TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::FANCY,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	// init text
	m_selectedTabText.setPosition(sf::Vector2f(GUIConstants::LEFT + GUIConstants::TEXT_OFFSET, GUIConstants::TOP + GUIConstants::TEXT_OFFSET));
	m_selectedTabText.setColor(sf::Color::White);
	m_selectedTabText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	// tabbar
	int nTabs = 5;
	float width = nTabs * BUTTON_SIZE.x;
	float height = BUTTON_SIZE.y;
	float x = GUIConstants::LEFT + 0.5f * (Spellbook::WIDTH - width);
	float y = GUIConstants::TOP + GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + 2 * MARGIN;

	m_tabBar = new TexturedTabBar(sf::FloatRect(x, y, width, height), nTabs);

	m_tabBar->getTabButton(0)->setTexture(g_resourceManager->getTexture(ResourceID::Texture_gems), sf::IntRect(50, 0, 50, 50));
	m_tabBar->getTabButton(1)->setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_elemental), sf::IntRect(50, 50, 400, 400));
	m_tabBar->getTabButton(1)->setTextureColor(CENDRIC_COLOR_ELEMENTAL);
	m_tabBar->getTabButton(2)->setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_divine), sf::IntRect(50, 50, 400, 400));
	m_tabBar->getTabButton(2)->setTextureColor(CENDRIC_COLOR_DIVINE);
	m_tabBar->getTabButton(3)->setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_necromancy), sf::IntRect(50, 50, 400, 400));
	m_tabBar->getTabButton(3)->setTextureColor(CENDRIC_COLOR_NECROMANCY);
	m_tabBar->getTabButton(4)->setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_twilight), sf::IntRect(50, 50, 400, 400));
	m_tabBar->getTabButton(4)->setTextureColor(CENDRIC_COLOR_TWILIGHT);


	// fill the helper map
	m_typeMap.insert({
		{ SpellType::Twilight, &m_twilightSlots },
		{ SpellType::Divine, &m_divineSlots },
		{ SpellType::Elemental, &m_elementalSlots },
		{ SpellType::Necromancy, &m_necromancySlots }
	});

	selectTab(SpellType::VOID);

	delete m_weaponWindow;
	m_weaponWindow = new WeaponWindow(m_core, m_isModifiable);

	reload();
}

Spellbook::~Spellbook() {
	m_typeMap.clear();
	delete m_window;
	delete m_weaponWindow;
	delete m_currentModifierClone;
	delete m_currentSpellClone;
	delete m_tabBar;
	clearAllSlots();
}

void Spellbook::clearAllSlots() {
	m_modifierSlots.clear();
	m_modifierTexts.clear();
	m_necromancySlots.clear();
	m_elementalSlots.clear();
	m_divineSlots.clear();
	m_twilightSlots.clear();
	m_selectedModifierSlot = nullptr;
	m_selectedSpellSlot = nullptr;
}

void Spellbook::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_tabBar->update(frameTime);
	int activeIndex = m_tabBar->getActiveTabIndex();
	SpellType type;
	if (activeIndex == 0) {
		type = SpellType::VOID;
	}
	else if (activeIndex == 1) {
		type = SpellType::Elemental;
	}
	else if (activeIndex == 2) {
		type = SpellType::Divine;
	}
	else if (activeIndex == 3) {
		type = SpellType::Necromancy;
	}
	else if (activeIndex == 4) {
		type = SpellType::Twilight;
	}

	if (m_tabBar->getTabButton(activeIndex)->isClicked() && m_currentTab != type) {
		selectTab(type);
	}

	// update weapon part
	m_weaponWindow->update(frameTime);

	if (m_currentTab == SpellType::VOID) {
		// handle gems
		for (auto& it : m_modifierSlots) {
			it.update(frameTime);
			if (it.isClicked()) {
				selectModifierSlot(&it);
				return;
			}
		}
	}
	else {
		// handle spells
		for (auto& it : *(m_typeMap[m_currentTab])) {
			it.first.update(frameTime);
			if (it.first.isClicked()) {
				selectSpellSlot(&it.first);
				return;
			}
		}
	}

	if (!m_isModifiable) return;

	handleDragAndDrop();
}

void Spellbook::selectModifierSlot(ModifierSlot* selectedSlot) {
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;

	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedModifierSlot) return;
	if (m_selectedModifierSlot != nullptr) {
		m_selectedModifierSlot->deselect();
	}
	m_selectedModifierSlot = selectedSlot;
	m_selectedModifierSlot->select();
}

void Spellbook::selectSpellSlot(SpellSlot* selectedSlot) {
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;

	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedSpellSlot) return;
	if (m_selectedSpellSlot != nullptr) {
		m_selectedSpellSlot->deselect();
	}
	m_selectedSpellSlot = selectedSlot;
	m_selectedSpellSlot->select();
}

void Spellbook::handleDragAndDrop() {
	if (!m_hasDraggingStarted) return;
	if (!(g_inputController->isMousePressedLeft())) {
		if (m_selectedModifierSlot != nullptr) {
			m_selectedModifierSlot->activate();
		}
		if (m_selectedSpellSlot != nullptr) {
			m_selectedSpellSlot->activate();
		}
		m_weaponWindow->notifyModifierDrop(m_currentModifierClone);
		m_weaponWindow->notifySpellDrop(m_currentSpellClone);
		m_weaponWindow->highlightSpellSlots(SpellType::VOID, false);
		m_weaponWindow->highlightModifierSlots(SpellModifierType::VOID, false);
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
			m_currentModifierClone = nullptr;
			delete m_currentSpellClone;
			m_currentSpellClone = nullptr;
			if (m_selectedModifierSlot != nullptr) {
				m_currentModifierClone = new SlotClone(m_selectedModifierSlot);
				m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIZE / 2.f, InventorySlot::SIZE / 2.f));
				m_selectedModifierSlot->deactivate();
				m_weaponWindow->highlightModifierSlots(m_selectedModifierSlot->getModifier().type, true);
			}
			else if (m_selectedSpellSlot != nullptr) {
				m_currentSpellClone = new SlotClone(m_selectedSpellSlot);
				m_currentSpellClone->setPosition(mousePos - sf::Vector2f(SpellSlot::SIZE / 2.f, SpellSlot::SIZE / 2.f));
				m_selectedSpellSlot->deactivate();
				m_weaponWindow->highlightSpellSlots(m_selectedSpellSlot->getSpellType(), true);
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

bool Spellbook::isVisible() const {
	return m_isVisible;
}

void Spellbook::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_selectedTabText);

	if (m_currentTab == SpellType::VOID) {
		for (auto& it : m_modifierSlots) {
			it.render(target);
		}
		for (auto& it : m_modifierTexts) {
			target.draw(it);
		}
	}
	else {
		for (auto& it : *m_typeMap[m_currentTab]) {
			it.first.render(target);
			it.first.renderAfterForeground(target);
			target.draw(it.second.first);
			target.draw(it.second.second);
		}
	}

	m_tabBar->render(target);

	m_weaponWindow->render(target);

	if (m_currentModifierClone != nullptr) {
		m_currentModifierClone->render(target);
	}
	if (m_currentSpellClone != nullptr) {
		m_currentSpellClone->render(target);
	}

	if (m_currentTab == SpellType::VOID) {
		for (auto& it : m_modifierSlots) {
			it.renderAfterForeground(target);
		}
	}
}

void Spellbook::selectTab(SpellType type) {
	if (m_selectedModifierSlot != nullptr) {
		m_selectedModifierSlot->deselect();
		m_selectedModifierSlot = nullptr;
	}
	if (m_selectedSpellSlot != nullptr) {
		m_selectedSpellSlot->deselect();
		m_selectedSpellSlot = nullptr;
	}
	m_currentTab = type;
	switch (type) {
	case SpellType::VOID:
		m_selectedTabText.setString(g_textProvider->getText("Gems"));
		break;
	case SpellType::Twilight:
	case SpellType::Necromancy:
	case SpellType::Divine:
	case SpellType::Elemental:
		m_selectedTabText.setString(g_textProvider->getText(EnumNames::getSpellTypeName(type)));
		break;
	default:
		break;
	}
	// center text
	m_selectedTabText.setPosition(
		m_window->getPosition().x +
		WIDTH / 2 -
		m_selectedTabText.getLocalBounds().width / 2, m_selectedTabText.getPosition().y);
}

void Spellbook::reload() {
	// reload slots
	clearAllSlots();
	calculateModifierSlots();
	calculateSpellSlots();

	// reload weapon
	m_weaponWindow->reload();
}

void Spellbook::calculateModifierSlots() {
	float yOffset = GUIConstants::TOP + SPELL_OFFSET;
	float xOffset = GUIConstants::LEFT + 2 * GUIConstants::TEXT_OFFSET;
	float modifierXOffset = 213.f;
	float textYOffset = SpellSlot::ICON_SIZE / 2.f - GUIConstants::CHARACTER_SIZE_S / 2.f;
	for (auto& it : m_core->getData().modfiersLearned) {
		BitmapText text;
		text.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
		text.setColor(CENDRIC_COLOR_WHITE);
		text.setPosition(sf::Vector2f(xOffset, yOffset + textYOffset));
		text.setString(g_textProvider->getText(EnumNames::getSpellModifierTypeName(it.first)));
		m_modifierTexts.push_back(text);
		for (int i = 0; i < it.second; i++) {
			SpellModifier modifier;
			modifier.level = i + 1;
			modifier.type = it.first;
			ModifierSlot slot(modifier);
			slot.setPosition(sf::Vector2f(modifierXOffset + (i * (ModifierSlot::SIZE + MARGIN)), yOffset));
			m_modifierSlots.push_back(slot);
		}
		yOffset += ModifierSlot::SIZE + 6.f;
	}
}

void Spellbook::calculateSpellSlots() {
	float yOffset = GUIConstants::TOP + SPELL_OFFSET;
	float xOffset = GUIConstants::LEFT + GUIConstants::TEXT_OFFSET / 2.f + SpellSlot::ICON_OFFSET;

	for (auto& it : m_core->getData().spellsLearned) {
		for (auto& it2 : it.second) {
			SpellSlot slot = SpellSlot(it2);
			slot.setPosition(sf::Vector2f(xOffset, yOffset));

			BitmapText text;
			text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
			text.setColor(CENDRIC_COLOR_WHITE);
			text.setString(g_textProvider->getText(EnumNames::getSpellIDName(it2)));
			text.setPosition(sf::Vector2f(xOffset + SpellSlot::ICON_SIZE + SpellSlot::ICON_OFFSET + MARGIN, yOffset));

			BitmapText textDesc;
			textDesc.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
			textDesc.setColor(CENDRIC_COLOR_LIGHT_GREY);
			textDesc.setString(g_textProvider->getCroppedText(
				EnumNames::getSpellIDName(it2) + "Desc", GUIConstants::CHARACTER_SIZE_S,
				static_cast<int>(WIDTH - (SpellSlot::SIZE + 4 * MARGIN))));
			textDesc.setPosition(sf::Vector2f(xOffset + SpellSlot::ICON_SIZE + SpellSlot::ICON_OFFSET + MARGIN, yOffset + GUIConstants::CHARACTER_SIZE_M + 4.f));

			std::pair<BitmapText, BitmapText> texts = std::pair<BitmapText, BitmapText>(text, textDesc);
			m_typeMap.at(it.first)->push_back(std::pair<SpellSlot, std::pair<BitmapText, BitmapText>>(slot, texts));
			yOffset += SpellSlot::SIZE + MARGIN;
		}

		yOffset = GUIConstants::TOP + SPELL_OFFSET;
	}
}

void Spellbook::show() {
	m_isVisible = true;
	m_weaponWindow->show();
}

void Spellbook::hide() {
	m_isVisible = false;
	//m_weaponWindow->hide();
	delete m_currentModifierClone;
	m_currentModifierClone = nullptr;
	delete m_currentSpellClone;
	m_currentSpellClone = nullptr;
	m_isDragging = false;
	m_hasDraggingStarted = false;
}