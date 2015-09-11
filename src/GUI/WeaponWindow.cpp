#include "GUI/WeaponWindow.h"
#include "MapInterface.h"

WeaponWindow::WeaponWindow(MapInterface* _interface)
{
	m_mapInterface = _interface;
	m_core = _interface->getCore();

	init();
}

void WeaponWindow::reload()
{
	// reload slot and text
	delete m_weaponSlot;
	m_weapon = m_core->getWeapon();
	if (m_weapon == nullptr)
	{
		m_weaponSlot = new InventorySlot(g_resourceManager->getTexture(ResourceID::Texture_equipmentplaceholders), sf::Vector2i(0, 0));
		m_weaponName.setString(g_textProvider->getText("NoWeapon"));
		m_weaponDescription.setString("");
	}
	else
	{
		m_weaponSlot = new InventorySlot(*m_weapon, -1);
		m_weaponName.setString(g_textProvider->getText(m_weapon->getID()));
		m_weaponDescription.setString(g_textProvider->getCroppedText(m_weapon->getDescription(), CHARACTER_SIZE, 
			static_cast<int>(WIDTH - (TEXT_OFFSET + 2 * MARGIN + InventorySlot::SIDE_LENGTH)))); 
	}
	m_weaponSlot->setPosition(sf::Vector2f(LEFT + TEXT_OFFSET, TOP + TEXT_OFFSET));

	clearAllSlots();

	if (m_weapon == nullptr) return;

	float xOffset = LEFT + TEXT_OFFSET + 2 * MARGIN;
	float yOffset = std::max(
		m_weaponSlot->getPosition().y + InventorySlot::SIDE_LENGTH,
		m_weaponDescription.getPosition().y + m_weaponDescription.getLocalBounds().height)
		+ 3 * MARGIN;
	for (auto& it : m_weapon->getWeaponSlots())
	{
		SpellSlot spellSlot = SpellSlot();
		if (it.first.second == SpellID::VOID)
		{
			spellSlot = SpellSlot(it.first.first);
		}
		else
		{
			spellSlot = SpellSlot(it.first.second);
		}
		spellSlot.setPosition(sf::Vector2f(xOffset, yOffset) + sf::Vector2f(SpellSlot::RADIUS, SpellSlot::RADIUS));
		xOffset += 2 * SpellSlot::RADIUS + 2 * MARGIN;

		std::vector<ModifierSlot> modifiers;
		for (auto& it2 : it.second.second)
		{
			modifiers.push_back(ModifierSlot(it2.second));
		}
		int emptyModifierCount = it.second.first - static_cast<int>(modifiers.size());
		for (int i = 0; i < emptyModifierCount; i++)
		{
			modifiers.push_back(ModifierSlot());
		}
		for (auto& it2 : modifiers)
		{
			it2.setPosition(sf::Vector2f(xOffset, yOffset));
			xOffset += ModifierSlot::SIDE_LENGTH + MARGIN;
		}

		yOffset += 2 * SpellSlot::RADIUS + 2 * MARGIN;
		xOffset = LEFT + TEXT_OFFSET + 2 * MARGIN;
		m_weaponSlots.push_back(std::pair<SpellSlot, std::vector<ModifierSlot>>({spellSlot, modifiers}));
	}
}

void WeaponWindow::init()
{
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		CENDRIC_COLOR_TRANS_BLACK, // main
		sf::Color::Transparent, // back
		CENDRIC_COLOR_LIGHT_PURPLE); // ornament

	m_weaponName.setCharacterSize(12);
	m_weaponName.setColor(CENDRIC_COLOR_WHITE);
	m_weaponName.setPosition(sf::Vector2f(LEFT + TEXT_OFFSET + MARGIN + InventorySlot::SIDE_LENGTH, TOP + TEXT_OFFSET));

	m_weaponDescription.setCharacterSize(CHARACTER_SIZE);
	m_weaponDescription.setColor(CENDRIC_COLOR_LIGHT_GREY);
	m_weaponDescription.setPosition(sf::Vector2f(LEFT + TEXT_OFFSET + MARGIN + InventorySlot::SIDE_LENGTH, TOP + TEXT_OFFSET + 2 * CHARACTER_SIZE));

	reload();
}

WeaponWindow::~WeaponWindow()
{
	delete m_window;
	delete m_currentModifierClone;
	delete m_weaponSlot;
	clearAllSlots();
}

void WeaponWindow::clearAllSlots()
{
	m_weaponSlots.clear();
	m_selectedModifierSlot = nullptr;
}

void WeaponWindow::update(const sf::Time& frameTime)
{
	if (!m_isVisible) return;

	for (auto& it : m_weaponSlots)
	{
		for (auto& it2 : it.second)
		{
			it2.update(frameTime);
		}
	}

	handleDragAndDrop();
}

void WeaponWindow::selectModifierSlot(ModifierSlot* selectedSlot)
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

void WeaponWindow::handleDragAndDrop()
{
	if (!m_hasDraggingStarted) return;
	if (!(g_inputController->isMousePressedLeft()))
	{
		if (m_selectedModifierSlot != nullptr)
		{
			m_selectedModifierSlot->activate();
		}
		delete m_currentModifierClone;
		m_currentModifierClone = nullptr;
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
			m_currentModifierClone = new ModifierSlotClone(m_selectedModifierSlot);
			m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIDE_LENGTH / 2.f, InventorySlot::SIDE_LENGTH / 2.f));
			m_selectedModifierSlot->deactivate();
		}
	}
	else
	{
		m_currentModifierClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIDE_LENGTH / 2.f, InventorySlot::SIDE_LENGTH / 2.f));
	}
}

bool WeaponWindow::isVisible() const
{
	return m_isVisible;
}

void WeaponWindow::render(sf::RenderTarget& target)
{
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_weaponName);
	target.draw(m_weaponDescription);
	m_weaponSlot->render(target);

	for (auto& it : m_weaponSlots)
	{
		it.first.render(target);
		for (auto& it2 : it.second)
		{
			it2.render(target);
		}
	}

	if (m_currentModifierClone != nullptr)
	{
		m_currentModifierClone->render(target);
	}
}

void WeaponWindow::show()
{
	m_isVisible = true;
}

void WeaponWindow::hide()
{
	m_isVisible = false;
	delete m_currentModifierClone;
	m_currentModifierClone = nullptr;
	m_isDragging = false;
	m_hasDraggingStarted = false;
}