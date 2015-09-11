#include "GUI/Spellbook.h"
#include "MapInterface.h"
#include "GUI/WeaponWindow.h"

float Spellbook::WIDTH = (WINDOW_WIDTH - Spellbook::LEFT - 20.f) / 3.f;
float Spellbook::HEIGHT = WINDOW_HEIGHT - 150.f;
float Spellbook::TOP = 100.f;
float Spellbook::LEFT = 50.f;

Spellbook::Spellbook(MapInterface* _interface)
{
	m_mapInterface = _interface;
	m_core = _interface->getCore();

	init();
}

void Spellbook::init()
{
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		CENDRIC_COLOR_TRANS_BLACK, // main
		sf::Color::Transparent, // back
		CENDRIC_COLOR_LIGHT_PURPLE); // ornament

	// init text
	m_selectedTabText.setPosition(sf::Vector2f(LEFT + TEXT_OFFSET, TOP + TEXT_OFFSET));
	m_selectedTabText.setColor(sf::Color::White);
	m_selectedTabText.setCharacterSize(CHARACTER_SIZE);

	// init buttons
	TexturedButton button = TexturedButton(sf::FloatRect(0, 0, BUTTON_SIZE.x, BUTTON_SIZE.y));
	button.setTexture(g_resourceManager->getTexture(ResourceID::Texture_gems), sf::IntRect(50, 0, 50, 50));
	m_tabs.push_back(std::pair<TexturedButton, SpellType>(button, SpellType::VOID));
	button.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_elemental), sf::IntRect(50, 50, 400, 400));
	button.setTextureColor(CENDRIC_COLOR_ELEMENTAL);
	m_tabs.push_back(std::pair<TexturedButton, SpellType>(button, SpellType::Elemental));
	button.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_divine), sf::IntRect(50, 50, 400, 400));
	button.setTextureColor(CENDRIC_COLOR_DIVINE);
	m_tabs.push_back(std::pair<TexturedButton, SpellType>(button, SpellType::Divine));
	button.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_illusion), sf::IntRect(50, 50, 400, 400));
	button.setTextureColor(CENDRIC_COLOR_ILLUSION);
	m_tabs.push_back(std::pair<TexturedButton, SpellType>(button, SpellType::Illusion));
	button.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_necromancy), sf::IntRect(50, 50, 400, 400));
	button.setTextureColor(CENDRIC_COLOR_NECROMANCY);
	m_tabs.push_back(std::pair<TexturedButton, SpellType>(button, SpellType::Necromancy));
	button.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_twilight), sf::IntRect(50, 50, 400, 400));
	button.setTextureColor(CENDRIC_COLOR_TWILIGHT);
	m_tabs.push_back(std::pair<TexturedButton, SpellType>(button, SpellType::Twilight));

	float xOffset = LEFT + TEXT_OFFSET;
	float yOffset = TOP + TEXT_OFFSET + CHARACTER_SIZE + 2 * MARGIN;
	float buttonMargin = (WIDTH - 6 * BUTTON_SIZE.x - 2 * TEXT_OFFSET) / 5.f;

	for (auto& it : m_tabs)
	{
		it.first.setPosition(sf::Vector2f(xOffset, yOffset));
		it.first.setBackgroundLayerColor(sf::Color::Transparent);
		it.first.setMainLayerColor(CENDRIC_COLOR_TRANS_BLACK);
		it.first.setOrnamentLayerColor(CENDRIC_COLOR_DARK_PURPLE);
		it.first.setMouseOverColor(CENDRIC_COLOR_LIGHT_PURPLE);
		xOffset += BUTTON_SIZE.x + buttonMargin;
	}

	// fill the helper map
	m_typeMap.insert({
		{ SpellType::Twilight, &m_twilightSlots },
		{ SpellType::Divine, &m_divineSlots },
		{ SpellType::Elemental, &m_elementalSlots },
		{ SpellType::Illusion, &m_illusionSlots },
		{ SpellType::Necromancy, &m_necromancySlots }
	});

	selectTab(SpellType::VOID);

	delete m_weaponWindow;
	m_weaponWindow = new WeaponWindow(m_mapInterface);
}

Spellbook::~Spellbook()
{
	delete m_window;
	delete m_weaponWindow;
	delete m_currentModifierClone;
	delete m_currentSpellClone;
	clearAllSlots();
}

void Spellbook::clearAllSlots()
{
	m_modifierSlots.clear();
	m_modifierTexts.clear();
	m_necromancySlots.clear();
	m_elementalSlots.clear();
	m_divineSlots.clear();
	m_illusionSlots.clear();
	m_twilightSlots.clear();
	m_selectedModifierSlot = nullptr;
}

void Spellbook::update(const sf::Time& frameTime)
{
	if (!m_isVisible) return;

	if (m_currentTab == SpellType::VOID)
	{
		// handle gems
		for (auto& it : m_modifierSlots)
		{
			it.update(frameTime);
			if (it.isClicked())
			{
				selectModifierSlot(&it);
				return;
			}
		}
	}
	else
	{
		// handle spells
		for (auto& it : *(m_typeMap[m_currentTab]))
		{
			it.first.update(frameTime);
			if (it.first.isClicked())
			{
				selectSpellSlot(&it.first);
				return;
			}
		}
	}

	for (auto& it : m_tabs)
	{
		it.first.update(frameTime);
		if (it.first.isClicked() && m_currentTab != it.second)
		{
			selectTab(it.second);
			return;
		}
	}

	// update weapon part
	m_weaponWindow->update(frameTime);

	handleDragAndDrop();
}

void Spellbook::selectModifierSlot(ModifierSlot* selectedSlot)
{
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;

	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedModifierSlot) return;
	if (m_selectedModifierSlot != nullptr)
	{
		m_selectedModifierSlot->deselect();
	}
	m_selectedModifierSlot = selectedSlot;
	m_selectedModifierSlot->select();
}

void Spellbook::selectSpellSlot(SpellSlot* selectedSlot)
{
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;

	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedSpellSlot) return;
	if (m_selectedSpellSlot != nullptr)
	{
		m_selectedSpellSlot->deselect();
	}
	m_selectedSpellSlot = selectedSlot;
	m_selectedSpellSlot->select();
}

void Spellbook::handleDragAndDrop()
{
	if (!m_hasDraggingStarted) return;
	if (!(g_inputController->isMousePressedLeft()))
	{
		if (m_selectedModifierSlot != nullptr)
		{
			m_selectedModifierSlot->activate();
		}
		if (m_selectedSpellSlot != nullptr)
		{
			m_selectedSpellSlot->activate();
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
	if (!m_isDragging)
	{
		if (DRAG_DISTANCE < std::sqrt(
			(mousePos.x - m_startMousePosition.x) * (mousePos.x - m_startMousePosition.x) +
			(mousePos.y - m_startMousePosition.y) * (mousePos.y - m_startMousePosition.y)))
		{
			m_isDragging = true;
			delete m_currentModifierClone;
			delete m_currentSpellClone;
			if (m_selectedModifierSlot != nullptr)
			{
				m_currentModifierClone = new ModifierSlotClone(m_selectedModifierSlot);
				m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIDE_LENGTH / 2.f, InventorySlot::SIDE_LENGTH / 2.f));
				m_selectedModifierSlot->deactivate();
			}
			else if (m_selectedSpellSlot != nullptr)
			{
				m_currentSpellClone = new SpellSlotClone(m_selectedSpellSlot);
				m_currentSpellClone->setPosition(mousePos);
				m_selectedSpellSlot->deactivate();
			}
		}
	}
	else
	{
		if (m_currentModifierClone != nullptr)
			m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIDE_LENGTH / 2.f, InventorySlot::SIDE_LENGTH / 2.f));
		if (m_currentSpellClone != nullptr)
			m_currentSpellClone->setPosition(mousePos);
	}
}

bool Spellbook::isVisible() const
{
	return m_isVisible;
}

void Spellbook::render(sf::RenderTarget& target)
{
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_selectedTabText);
	
	if (m_currentTab == SpellType::VOID)
	{
		for (auto& it : m_modifierSlots)
		{
			it.render(target);
		}
		for (auto& it : m_modifierTexts)
		{
			target.draw(it);
		}
	}
	else
	{
		for (auto& it : *m_typeMap[m_currentTab])
		{
			it.first.render(target);
			target.draw(it.second);
		}
	}
	
	for (auto& it : m_tabs)
	{
		it.first.render(target);
	}

	m_weaponWindow->render(target);
	
	if (m_currentModifierClone != nullptr)
	{
		m_currentModifierClone->render(target);
	}
	if (m_currentSpellClone != nullptr)
	{
		m_currentSpellClone->render(target);
	}
}

void Spellbook::selectTab(SpellType type)
{
	if (m_selectedModifierSlot != nullptr)
	{
		m_selectedModifierSlot->deselect();
		m_selectedModifierSlot = nullptr;
	}
	m_currentTab = type;
	switch (type)
	{
	case SpellType::VOID:
		m_selectedTabText.setString(g_textProvider->getText("Gems"));
		break;
	case SpellType::Twilight:
	case SpellType::Necromancy:
	case SpellType::Divine:
	case SpellType::Elemental:
	case SpellType::Illusion:
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
	// color selected tab
	for (auto& it : m_tabs)
	{
		if (it.second == m_currentTab)
		{
			it.first.setMainLayerColor(CENDRIC_COLOR_LIGHT_GREY);
		}
		else
		{
			it.first.setMainLayerColor(CENDRIC_COLOR_TRANS_BLACK);
		}
	}
}

void Spellbook::reload()
{
	// reload slots
	clearAllSlots();
	calculateModifierSlots();
	calculateSpellSlots();

	// reload weapon
	m_weaponWindow->reload();
}

void Spellbook::calculateModifierSlots()
{
	float yOffset = TOP + TEXT_OFFSET + 2 * CHARACTER_SIZE + 2 * MARGIN + 2 * BUTTON_SIZE.y;
	float xOffset = LEFT + TEXT_OFFSET;
	int maxY = 4;
	int y = 1;
	for (auto& it : m_core->getData().modfiersLearned)
	{
		BitmapText text;
		text.setCharacterSize(CHARACTER_SIZE);
		text.setColor(CENDRIC_COLOR_WHITE);
		text.setPosition(sf::Vector2f(xOffset, yOffset));
		text.setString(g_textProvider->getText(EnumNames::getSpellModifierTypeName(it.first)));
		m_modifierTexts.push_back(text);
		yOffset += CHARACTER_SIZE * 2;
		for (int i = 0; i < it.second; i++)
		{
			SpellModifier modifier;
			modifier.level = i + 1;
			modifier.type = it.first;
			ModifierSlot slot(modifier);
			slot.setPosition(sf::Vector2f(xOffset + (i * (ModifierSlot::SIDE_LENGTH + MARGIN)), yOffset));
			m_modifierSlots.push_back(slot);
		}
		if (y >= maxY)
		{
			yOffset = TOP + TEXT_OFFSET + 2 * CHARACTER_SIZE + 2 * MARGIN + 2 * BUTTON_SIZE.y;
			xOffset = LEFT + WIDTH - (3 * ModifierSlot::SIDE_LENGTH + 2 * MARGIN + TEXT_OFFSET);
			y = 0;
		}
		else
		{
			y++;
			yOffset += CHARACTER_SIZE + ModifierSlot::SIDE_LENGTH;
		}
		
	}
}

void Spellbook::calculateSpellSlots()
{
	float yOffset = TOP + TEXT_OFFSET + 2 * CHARACTER_SIZE + 2 * MARGIN + 1 * BUTTON_SIZE.y;
	float xOffset = LEFT + TEXT_OFFSET;
	
	for (auto& it : m_core->getData().spellsLearned)
	{
		for (auto& it2 : it.second)
		{
			
			SpellSlot slot = SpellSlot(it2);
			slot.setPosition(sf::Vector2f(xOffset, yOffset) + sf::Vector2f(SpellSlot::RADIUS, SpellSlot::RADIUS));

			BitmapText text;
			text.setCharacterSize(CHARACTER_SIZE);
			text.setColor(CENDRIC_COLOR_WHITE);

			std::wstring spellString;
			spellString.append(g_textProvider->getText(EnumNames::getSpellIDName(it2)));
			spellString.append(L"\n");
			spellString.append(g_textProvider->getCroppedText(EnumNames::getSpellIDName(it2) + "Desc", CHARACTER_SIZE, static_cast<int>(WIDTH - (SpellSlot::RADIUS * 2 + 3 * MARGIN))));
			text.setString(spellString);
			text.setPosition(sf::Vector2f(xOffset + SpellSlot::RADIUS * 2 + 2 * MARGIN, yOffset));

			m_typeMap.at(it.first)->push_back(std::pair<SpellSlot, BitmapText>(slot, text));
			yOffset += SpellSlot::RADIUS * 2 + MARGIN * 2;
		}
		
		yOffset = TOP + TEXT_OFFSET + 2 * CHARACTER_SIZE + 2 * MARGIN + 1 * BUTTON_SIZE.y;
	}
}

void Spellbook::show()
{
	m_isVisible = true;
	m_weaponWindow->show();
}

void Spellbook::hide()
{
	m_isVisible = false;
	m_weaponWindow->hide();
	delete m_currentModifierClone;
	m_currentModifierClone = nullptr;
	m_isDragging = false;
	m_hasDraggingStarted = false;
}