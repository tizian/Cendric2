#include "GUI/MerchantWindow.h"
#include "MerchantInterface.h"

MerchantWindow::MerchantWindow(MerchantInterface* _interface) {
	m_interface = _interface;

	init();
}

void MerchantWindow::init() {
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::FANCY,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	// init text
	m_title.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET, LEFT + GUIConstants::TEXT_OFFSET));
	m_title.setColor(COLOR_WHITE);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText(m_interface->getMerchantID(), "npc"));
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

MerchantWindow::~MerchantWindow() {
	delete m_window;
	delete m_descriptionWindow;
	delete m_cancelButton;
	clearAllSlots();
}

void MerchantWindow::clearAllSlots() {
	m_items.clear();
	m_selectedSlotId = "";
}

void MerchantWindow::notifyChange(const std::string& itemID) {
	ItemBean bean = g_databaseManager->getItemBean(itemID);
	if (bean.status != BeanStatus::Filled) return;

	// search for the slot
	if (m_items.find(bean.item_id) != m_items.end()) {
		if (m_interface->getMerchantData().wares.find(itemID) == m_interface->getMerchantData().wares.end()) {
			// the item was removed. check if it is selected.
			if (m_selectedSlotId.compare(bean.item_id) == 0) {
				deselectCurrentSlot();
			}
			m_items.erase(bean.item_id);
			calculateSlotPositions();
		}
		else {
			m_items.at(bean.item_id).setAmount(m_interface->getMerchantData().wares.at(itemID));
		}
		return;
	}

	// the slot for that item has not been found. The slot is added with the current amount in the core
	if (m_interface->getMerchantData().wares.find(itemID) == m_interface->getMerchantData().wares.end()) return;

	m_items.insert({ bean.item_id, InventorySlot(Item(bean.item_id), m_interface->getMerchantData().wares.at(itemID)) });

	calculateSlotPositions();
}

void MerchantWindow::update(const sf::Time& frameTime) {
	// check whether an item was selected
	for (auto& slot : m_items) {
		slot.second.update(frameTime);
		if (slot.second.isClicked()) {
			selectSlot(slot.second.getItemID());
			return;
		}
		if (slot.second.isRightClicked()) {
			m_interface->buyItem(slot.second.getItem());
			break;
		}
	}

	m_cancelButton->update(frameTime);
	if (g_inputController->isKeyJustPressed(Key::Escape) || m_cancelButton->isClicked()) {
		m_interface->completeTrade();
	}
}

void MerchantWindow::selectSlot(const std::string& selectedSlotId) {
	if (selectedSlotId.empty()) {
		deselectCurrentSlot();
		return;
	}
	
	if (selectedSlotId.compare(m_selectedSlotId) == 0) return;

	deselectCurrentSlot();
	m_selectedSlotId = selectedSlotId;

	InventorySlot* selectedSlot = getSelectedSlot();
	
	if (selectedSlot != nullptr) {
		selectedSlot->select();
		showDescription(selectedSlot->getItem());
	}
}

void MerchantWindow::deselectCurrentSlot() {
	InventorySlot* slot = getSelectedSlot();
	m_selectedSlotId = "";
	m_descriptionWindow->hide();
	if (slot != nullptr) {
		slot->deselect();
	}
}

InventorySlot* MerchantWindow::getSelectedSlot() {
	if (m_selectedSlotId.empty()) return nullptr;
	if (m_items.find(m_selectedSlotId) == m_items.end()) return nullptr;
	return &m_items.at(m_selectedSlotId);
}

void MerchantWindow::render(sf::RenderTarget& target) {
	m_window->render(target);
	target.draw(m_title);
	for (auto& it : m_items) {
		it.second.render(target);
		// it.renderAfterForeground(target); // uncomment for debug box
	}

	m_descriptionWindow->render(target);
	m_cancelButton->render(target);
}

void MerchantWindow::showDescription(const Item& item) {
	m_descriptionWindow->load(item);
	m_descriptionWindow->show();
	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x - MARGIN - m_descriptionWindow->getSize().x,
		m_window->getPosition().y + m_window->getSize().y - m_descriptionWindow->getSize().y);
	m_descriptionWindow->setPosition(pos);
}

void MerchantWindow::hideDescription() {
	m_descriptionWindow->hide();
}

void MerchantWindow::reload() {
	// reload items
	clearAllSlots();
	hideDescription();
	for (auto& it : m_interface->getMerchantData().wares) {
		ItemBean bean = g_databaseManager->getItemBean(it.first);
		if (bean.status != BeanStatus::Filled) {
			g_logger->logError("MerchantWindow", "Item not resolved: " + it.first);
			continue;
		}
		
		m_items.insert({ bean.item_id, InventorySlot(Item(bean.item_id), it.second) });
	}

	calculateSlotPositions();
}

void MerchantWindow::calculateSlotPositions() {
	float yOffset = TOP + 2 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M;
	float xOffset = LEFT + GUIConstants::TEXT_OFFSET;
	int y = 1;
	int x = 1;
	for (auto& it : m_items) {
		it.second.setPosition(sf::Vector2f(xOffset, yOffset));
		if (x + 1 > SLOT_COUNT_X) {
			x = 1;
			xOffset = LEFT + GUIConstants::TEXT_OFFSET;
			y++;
			yOffset += MARGIN + InventorySlot::SIZE;
		}
		else {
			x++;
			xOffset += MARGIN + InventorySlot::SIZE;
		}
	}
}
