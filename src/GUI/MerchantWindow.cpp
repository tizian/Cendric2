#include "GUI/MerchantWindow.h"
#include "MerchantInterface.h"

MerchantWindow::MerchantWindow(MerchantInterface* _interface)
{
	m_interface = _interface;

	init();
}

void MerchantWindow::init()
{
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	// init text
	m_title.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET, LEFT + GUIConstants::TEXT_OFFSET));
	m_title.setColor(sf::Color::White);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText(m_interface->getMerchantID()));
	m_title.setPosition(
		m_window->getPosition().x +
		WIDTH / 2 -
		m_title.getLocalBounds().width / 2, m_window->getPosition().y + GUIConstants::TEXT_OFFSET);

	// init Button
	m_cancelButton = new Button(sf::FloatRect(0.f, 0.f, 150.f, 30.f));
	m_cancelButton->setPosition(sf::Vector2f(LEFT + 1.5f * GUIConstants::TEXT_OFFSET, TOP + HEIGHT - 2.f * GUIConstants::TEXT_OFFSET));
	m_cancelButton->setText("Cancel");
	m_cancelButton->setCharacterSize(GUIConstants::CHARACTER_SIZE_S);

	m_descriptionWindow = new MerchantItemDescriptionWindow(m_interface->getMerchantData().multiplier);

	reload();
}

MerchantWindow::~MerchantWindow()
{
	delete m_window;
	delete m_descriptionWindow;
	delete m_cancelButton;
	clearAllSlots();
}

void MerchantWindow::clearAllSlots()
{
	m_items.clear();
	m_selectedSlot = nullptr;
}

void MerchantWindow::notifyChange(const std::string& itemID)
{
	const ItemBean* bean = g_resourceManager->getItemBean(itemID);
	if (bean == nullptr) return;

	// search for the slot
	std::vector<InventorySlot>::iterator it = m_items.begin();
	while (it != m_items.end())
	{
		if (bean->id.compare((*it).getItemID()) == 0)
		{
			// the slot has been found.
			if (m_interface->getMerchantData().wares.find(itemID) == m_interface->getMerchantData().wares.end())
			{
				// the item was removed. check if it is selected.
				if (m_selectedSlot == &(*it))
				{
					m_selectedSlot = nullptr;
					m_descriptionWindow->hide();
				}
				m_items.erase(it);
				calculateSlotPositions(m_items);
			}
			else
			{
				(*it).setAmount(m_interface->getMerchantData().wares.at(itemID));
			}
			return;
		}
		it++;
	}

	// the slot for that item has not been found. The slot is added with the current amount in the core
	if (m_interface->getMerchantData().wares.find(itemID) == m_interface->getMerchantData().wares.end()) return;
	m_items.push_back(InventorySlot(Item(*bean), m_interface->getMerchantData().wares.at(itemID)));

	calculateSlotPositions(m_items);
}

void MerchantWindow::update(const sf::Time& frameTime)
{
	// check whether an item was selected
	for (auto& it : m_items)
	{
		it.update(frameTime);
		if (it.isClicked())
		{
			selectSlot(&it);
			return;
		}
		if (it.isRightClicked())
		{
			m_interface->buyItem(it.getItem());
			break;
		}
	}

	m_cancelButton->update(frameTime);
	if (m_cancelButton->isClicked())
	{
		m_interface->completeTrade();
	}
}

void MerchantWindow::selectSlot(InventorySlot* selectedSlot)
{
	if (selectedSlot == nullptr) return;
	if (selectedSlot == m_selectedSlot) return;
	if (m_selectedSlot != nullptr)
	{
		m_selectedSlot->deselect();
	}
	m_selectedSlot = selectedSlot;
	m_selectedSlot->select();
	showDescription(m_selectedSlot->getItem());
}

void MerchantWindow::render(sf::RenderTarget& target)
{
	m_window->render(target);
	target.draw(m_title);
	for (auto& it : m_items)
	{
		it.render(target);
		// it.renderAfterForeground(target); // uncomment for debug box
	}

	m_descriptionWindow->render(target);
	m_cancelButton->render(target);
}

void MerchantWindow::showDescription(const Item& item)
{
	m_descriptionWindow->load(item);
	m_descriptionWindow->show();
	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x - MARGIN - m_descriptionWindow->getSize().x,
		m_window->getPosition().y + m_window->getSize().y - m_descriptionWindow->getSize().y);
	m_descriptionWindow->setPosition(pos);
}

void MerchantWindow::hideDescription()
{
	m_descriptionWindow->hide();
}

void MerchantWindow::reload()
{
	// reload items
	clearAllSlots();
	hideDescription();
	for (auto& it : m_interface->getMerchantData().wares)
	{
		const ItemBean* bean = g_resourceManager->getItemBean(it.first);
		if (bean == nullptr)
		{
			g_logger->logError("MerchantWindow", "Item not resolved: " + it.first);
			continue;
		}
		m_items.push_back(InventorySlot(Item(*bean), it.second));
	}

	calculateSlotPositions(m_items);
}

void MerchantWindow::calculateSlotPositions(std::vector<InventorySlot>& slots)
{
	float yOffset = TOP + 2 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M;
	float xOffset = LEFT + GUIConstants::TEXT_OFFSET;
	int y = 1;
	int x = 1;
	for (auto& it : slots)
	{
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		if (x + 1 > SLOT_COUNT_X)
		{
			x = 1;
			xOffset = LEFT + GUIConstants::TEXT_OFFSET;
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
