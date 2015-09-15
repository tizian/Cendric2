#include "GUI/Inventory.h"
#include "LevelMainCharacter.h"
#include "LevelInterface.h"
#include "MapInterface.h"
#include "GUI/GUIConstants.h"

Inventory::Inventory(LevelInterface* _interface)
{
	m_levelInterface = _interface;
	m_core = _interface->getCore();
	
	init();
}

Inventory::Inventory(MapInterface* _interface)
{
	m_mapInterface = _interface;
	m_core = _interface->getCore();

	init();
}

void Inventory::init()
{
	// init window
	sf::FloatRect box(INVENTORY_LEFT, INVENTORY_TOP, INVENTORY_WIDTH, INVENTORY_HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	// init text
	m_selectedTabText.setPosition(sf::Vector2f(INVENTORY_LEFT + GUIConstants::TEXT_OFFSET, INVENTORY_TOP + GUIConstants::TEXT_OFFSET));
	m_selectedTabText.setColor(sf::Color::White);
	m_selectedTabText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	m_goldText.setPosition(sf::Vector2f(INVENTORY_LEFT + GUIConstants::TEXT_OFFSET, INVENTORY_TOP + INVENTORY_HEIGHT - GUIConstants::TEXT_OFFSET - GUIConstants::CHARACTER_SIZE_S));
	m_goldText.setColor(sf::Color::White);
	m_goldText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	// fill the helper map
	m_typeMap.insert({
		{ ItemType::Consumable, &m_consumableItems },
		{ ItemType::Misc, &m_miscItems },
		{ ItemType::Document, &m_documentItems },
		{ ItemType::Quest, &m_questItems },
		{ ItemType::Equipment_back, &m_equipmentItems },
		{ ItemType::Equipment_body, &m_equipmentItems },
		{ ItemType::Equipment_head, &m_equipmentItems },
		{ ItemType::Equipment_neck, &m_equipmentItems },
		{ ItemType::Equipment_ring_1, &m_equipmentItems },
		{ ItemType::Equipment_ring_2, &m_equipmentItems },
		{ ItemType::Equipment_weapon, &m_equipmentItems },
	});

	// init buttons
	TexturedButton button = TexturedButton(sf::FloatRect(0, 0, BUTTON_SIZE.x, BUTTON_SIZE.y));
	m_tabs.push_back(std::pair<TexturedButton, ItemType>(button, ItemType::Equipment_weapon));
	m_tabs.push_back(std::pair<TexturedButton, ItemType>(button, ItemType::Consumable));
	m_tabs.push_back(std::pair<TexturedButton, ItemType>(button, ItemType::Document));
	m_tabs.push_back(std::pair<TexturedButton, ItemType>(button, ItemType::Quest));
	m_tabs.push_back(std::pair<TexturedButton, ItemType>(button, ItemType::Misc));

	int textureOffset = 0;
	float xOffset = INVENTORY_LEFT + GUIConstants::TEXT_OFFSET;
	float yOffset = INVENTORY_TOP + GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + 2 * MARGIN;
	float buttonMargin = (INVENTORY_WIDTH - 5 * BUTTON_SIZE.x - 2 * GUIConstants::TEXT_OFFSET) / 4.f;

	for (auto& it : m_tabs)
	{
		it.first.setTexture(g_resourceManager->getTexture(ResourceID::Texture_inventorytabs), sf::IntRect(textureOffset, 0, 60, 35));
		it.first.setPosition(sf::Vector2f(xOffset, yOffset));
		it.first.setBackgroundLayerColor(CENDRIC_COLOR_TRANS_BLACK);
		it.first.setMainLayerColor(CENDRIC_COLOR_TRANS_BLACK);
		it.first.setOrnamentLayerColor(CENDRIC_COLOR_DARK_PURPLE);
		it.first.setMouseOverColor(CENDRIC_COLOR_LIGHT_PURPLE);
		xOffset += BUTTON_SIZE.x + buttonMargin;
		textureOffset += 60;
	}

	selectTab(ItemType::Consumable);

	m_equipment = new InventoryEquipment(m_core);
	reload();
}

Inventory::~Inventory()
{
	delete m_window;
	delete m_descriptionWindow;
	delete m_documentWindow;
	delete m_equipment;
	delete m_currentClone;
	clearAllSlots();
}

void Inventory::clearAllSlots()
{
	m_consumableItems.clear();
	m_equipmentItems.clear();
	m_questItems.clear();
	m_documentItems.clear();
	m_miscItems.clear();
	m_selectedSlot = nullptr;
}

void Inventory::update(const sf::Time& frameTime)
{
	if (!m_isVisible) return;

	// check whether an item was selected
	for (auto& it : *(m_typeMap[m_currentTab]))
	{
		it.update(frameTime);
		if (it.isClicked())
		{
			selectSlot(&it, false);
			return;
		}
		if (it.isRightClicked() && m_levelInterface != nullptr)
		{
			if (it.getItemType() == ItemType::Consumable)
				m_levelInterface->consumeItem(it.getItem());
			else if (it.getItemType() == ItemType::Document)
				showDocument(it.getItem());
			break;
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

	// update equipment part
	m_equipment->update(frameTime);
	selectSlot(m_equipment->getSelectedSlot(), true);

	handleDragAndDrop();
}

void Inventory::selectSlot(InventorySlot* selectedSlot, bool isEquipmentSlot)
{
	if (selectedSlot == nullptr) return;
	m_hasDraggingStarted = true;
	m_isEquipmentSlotDragged = isEquipmentSlot;
	m_startMousePosition = g_inputController->getDefaultViewMousePosition();
	if (selectedSlot == m_selectedSlot) return;
	if (m_selectedSlot != nullptr)
	{
		m_selectedSlot->deselect();
	}
	m_selectedSlot = selectedSlot;
	m_selectedSlot->select();
	hideDocument();
	showDescription(m_selectedSlot->getItem());
}

void Inventory::removeEquipmentItem()
{
	if (m_window->getBoundingBox()->intersects(*m_currentClone->getBoundingBox()))
	{
		m_core->equipItem("", m_currentClone->getItemType());
		reload();
	}
}

void Inventory::handleDragAndDrop()
{
	if (!m_hasDraggingStarted) return;
	if (!(g_inputController->isMousePressedLeft()))
	{
		if (m_selectedSlot != nullptr)
		{
			m_selectedSlot->activate();
			if (m_selectedSlot->getItemType() == ItemType::Consumable && m_levelInterface != nullptr)
			{
				m_levelInterface->notifyConsumableDrop(m_currentClone);
				m_levelInterface->highlightQuickslots(false);
			}
			else if (m_mapInterface != nullptr && m_currentClone != nullptr)
			{
				if (m_isEquipmentSlotDragged)
				{
					removeEquipmentItem();
				}
				else
				{
					if (m_equipment->notifyEquipmentDrop(m_currentClone))
					{
						reload();
					}
					m_equipment->highlightEquipmentSlot(m_currentClone->getItemType(), false);
				}
			}
		}
		delete m_currentClone;
		m_currentClone = nullptr;
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
			delete m_currentClone;
			m_currentClone = new InventorySlotClone(m_selectedSlot);
			m_currentClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIDE_LENGTH / 2.f, InventorySlot::SIDE_LENGTH / 2.f));
			m_selectedSlot->deactivate();
			if (m_selectedSlot->getItemType() == ItemType::Consumable && m_levelInterface != nullptr)
			{
				m_levelInterface->highlightQuickslots(true);
			}
			else if (m_mapInterface != nullptr && m_selectedSlot != nullptr && !m_isEquipmentSlotDragged)
			{
				m_equipment->highlightEquipmentSlot(m_selectedSlot->getItemType(), true);
			}
		}
	}
	else
	{
		m_currentClone->setPosition(mousePos - sf::Vector2f(InventorySlot::SIDE_LENGTH / 2.f, InventorySlot::SIDE_LENGTH / 2.f));
	}
}

bool Inventory::isVisible() const
{
	return m_isVisible;
}

void Inventory::render(sf::RenderTarget& target)
{
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_goldText);
	target.draw(m_selectedTabText);
	for (auto& it : *(m_typeMap[m_currentTab]))
	{
		it.render(target);
		// it.renderAfterForeground(target); // uncomment for debug box
	}

	for (auto& it : m_tabs)
	{
		it.first.render(target);
	}

	if (m_descriptionWindow != nullptr)
	{
		m_descriptionWindow->render(target);
	}
	if (m_documentWindow != nullptr)
	{
		m_documentWindow->render(target);
	}

	m_equipment->render(target);
	if (m_currentClone != nullptr)
	{
		m_currentClone->render(target);
	}
}

void Inventory::showDocument(const Item& item)
{
	delete m_documentWindow;
	m_documentWindow = new DocumentDescriptionWindow(item);

	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x + MARGIN + m_window->getSize().x,
		m_window->getPosition().y);
	if (m_descriptionWindow != nullptr)
	{
		pos.x += InventoryDescriptionWindow::WIDTH + MARGIN;
	}
	m_documentWindow->setPosition(pos);
}

void Inventory::showDescription(const Item& item)
{
	delete m_descriptionWindow;
	m_descriptionWindow = new InventoryDescriptionWindow(item);
	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x + MARGIN + m_window->getSize().x,
		m_window->getPosition().y);
	m_descriptionWindow->setPosition(pos);
	if (m_documentWindow != nullptr)
	{
		pos.x += InventoryDescriptionWindow::WIDTH + MARGIN;
		m_documentWindow->setPosition(pos);
	}
}

void Inventory::hideDescription()
{
	delete m_descriptionWindow;
	m_descriptionWindow = nullptr;
	if (m_documentWindow != nullptr)
	{
		m_documentWindow->setPosition(
			m_documentWindow->getPosition() - 
			sf::Vector2f(InventoryDescriptionWindow::WIDTH + MARGIN, 0.f));
	}
}

void Inventory::hideDocument()
{
	delete m_documentWindow;
	m_documentWindow = nullptr;
}

void Inventory::selectTab(ItemType type)
{
	hideDescription();
	hideDocument();
	if (m_selectedSlot != nullptr)
	{
		m_selectedSlot->deselect();
		m_selectedSlot = nullptr;
	}
	m_currentTab = type;
	switch (type)
	{
	case ItemType::Equipment_weapon:
		m_selectedTabText.setString(g_textProvider->getText("Equipment"));
		break;
	case ItemType::Consumable:
		m_selectedTabText.setString(g_textProvider->getText("Consumables"));
		break;
	case ItemType::Document:
		m_selectedTabText.setString(g_textProvider->getText("Documents"));
		break;
	case ItemType::Quest:
		m_selectedTabText.setString(g_textProvider->getText("QuestItems"));
		break;
	case ItemType::Misc:
		m_selectedTabText.setString(g_textProvider->getText("Miscellaneous"));
		break;
	default:
		break;
	}
	// center text
	m_selectedTabText.setPosition(
		m_window->getPosition().x + 
		INVENTORY_WIDTH / 2 - 
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

void Inventory::reload()
{
	// reload gold
	std::wstring gold = L"";
	gold.append(g_textProvider->getText("Gold"));
	gold.append(L": ");
	gold.append(std::to_wstring(m_core->getData().gold));
	gold.append(L"\n\n");
	m_goldText.setString(gold);
	
	// reload items
	clearAllSlots();
	hideDescription();
	hideDocument();
	m_core->loadItems();
	for (auto& it : m_core->getData().items)
	{
		const Item& item = m_core->getItem(it.first);
		if (m_typeMap[item.getType()] == nullptr) continue;
		m_typeMap[item.getType()]->push_back(InventorySlot(item, it.second));
	}

	calculateSlotPositions(m_consumableItems);
	calculateSlotPositions(m_equipmentItems);
	calculateSlotPositions(m_questItems);
	calculateSlotPositions(m_documentItems);
	calculateSlotPositions(m_miscItems);

	// reload equipment
	m_equipment->reload();
}

void Inventory::calculateSlotPositions(std::vector<InventorySlot>& slots)
{
	float yOffset = INVENTORY_TOP + 2 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + 2 * MARGIN + BUTTON_SIZE.y;
	float xOffset = INVENTORY_LEFT + GUIConstants::TEXT_OFFSET;
	int y = 1;
	int x = 1;
	for (auto& it : slots)
	{
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		if (x + 1 > SLOT_COUNT_X)
		{
			x = 1;
			xOffset = INVENTORY_LEFT + GUIConstants::TEXT_OFFSET;
			y++;
			yOffset += MARGIN + 2 * InventorySlot::MARGIN + InventorySlot::SIDE_LENGTH;
		}
		else
		{
			x++;
			xOffset += MARGIN + 2 * InventorySlot::MARGIN + InventorySlot::SIDE_LENGTH;
		}
	}
}

void Inventory::show()
{
	m_isVisible = true;
	m_equipment->show();
}

void Inventory::hide()
{
	m_isVisible = false;
	m_equipment->hide();
	delete m_currentClone;
	m_currentClone = nullptr;
	m_isDragging = false;
	m_hasDraggingStarted = false;
}