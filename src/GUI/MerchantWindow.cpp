#include "GUI/MerchantWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "Map/MerchantInterface.h"
#include "GlobalResource.h"

const int MerchantWindow::SLOT_COUNT_X = 3;
const int MerchantWindow::SLOT_COUNT_Y = 7;

const float MerchantWindow::ICON_MARGIN = 8.f;
const float MerchantWindow::WINDOW_MARGIN = 6.f;

const float MerchantWindow::SCROLL_WINDOW_LEFT = GUIConstants::TEXT_OFFSET;
const float MerchantWindow::SCROLL_WINDOW_TOP = 2 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M - WINDOW_MARGIN;
const float MerchantWindow::SCROLL_WINDOW_WIDTH = SLOT_COUNT_X * InventorySlot::SIZE + (SLOT_COUNT_X - 1) * ICON_MARGIN + 4 * WINDOW_MARGIN + ScrollBar::WIDTH;
const float MerchantWindow::SCROLL_WINDOW_HEIGHT = SLOT_COUNT_Y * InventorySlot::SIZE + (SLOT_COUNT_Y - 1) * ICON_MARGIN + 4 * WINDOW_MARGIN;

const float MerchantWindow::WIDTH = SCROLL_WINDOW_WIDTH + 2 * SCROLL_WINDOW_LEFT;
const float MerchantWindow::TOP = GUIConstants::TOP;
const float MerchantWindow::LEFT = WINDOW_WIDTH - WIDTH - GUIConstants::LEFT_BAR;

MerchantWindow::MerchantWindow(MerchantInterface* _interface) {
	m_interface = _interface;

	init();
}

void MerchantWindow::init() {
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, GUIConstants::GUI_WINDOW_HEIGHT);
	m_window = new Window(box,
		GUIOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&MerchantWindow::completeTrade, this));

	m_descriptionWindow = new MerchantItemDescriptionWindow(m_interface->getMerchantData());

	// init text
	m_title.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET, TOP + GUIConstants::TEXT_OFFSET));
	m_title.setColor(COLOR_WHITE);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText(removeDigits(m_interface->getMerchantID()), "npc"));
	m_title.setPosition(
		m_window->getPosition().x +
		WIDTH / 2 -
		m_title.getLocalBounds().width / 2, m_window->getPosition().y + GUIConstants::TEXT_OFFSET);

	// scrolling
	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP));

	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT, m_window);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, TOP + SCROLL_WINDOW_TOP));

	sf::FloatRect scrollBox(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	reload();
}

MerchantWindow::~MerchantWindow() {
	delete m_window;
	delete m_descriptionWindow;
	delete m_scrollBar;
	delete m_scrollHelper;
	clearAllSlots();
}

void MerchantWindow::clearAllSlots() {
	m_items.clear();
	m_selectedSlotId = "";
}

void MerchantWindow::completeTrade() {
	m_interface->completeTrade();
}

void MerchantWindow::notifyChange(const std::string& itemID) {
	auto item = g_resourceManager->getItem(itemID);
	if (!item) return;

	// search for the slot
	if (contains(m_items, itemID)) {
		if (!contains(m_interface->getMerchantData().wares, itemID)) {
			// the item was removed. check if it is selected.
			if (m_selectedSlotId == itemID) {
				deselectCurrentSlot();
			}
			m_items.erase(itemID);
			calculateSlotPositions();
		}
		else {
			m_items.at(itemID).setAmount(m_interface->getMerchantData().wares.at(itemID));
		}
		return;
	}

	// the slot for that item has not been found. The slot is added with the current amount in the core
	if (!contains(m_interface->getMerchantData().wares, itemID)) return;

	m_items.insert({ itemID, InventorySlot(itemID, m_interface->getMerchantData().wares.at(itemID)) });

	calculateSlotPositions();
}

void MerchantWindow::update(const sf::Time& frameTime) {
	m_scrollBar->update(frameTime);

	// check whether an item was selected
	for (auto& slot : m_items) {
		sf::Vector2f pos = slot.second.getPosition();
		if (pos.y < TOP + SCROLL_WINDOW_TOP ||
			pos.y + InventorySlot::SIZE > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) continue;
		slot.second.update(frameTime);
		if (slot.second.isMousedOver()) {
			selectSlot(slot.second.getItemID());
			if (slot.second.isRightClicked()) {
				m_interface->buyItem(slot.second.getItem());
				break;
			}
		}
	}

	calculateSlotPositions();
	
	if (g_inputController->isKeyJustPressed(Key::Escape)) {
		completeTrade();
	}

	m_window->update(frameTime);
}

void MerchantWindow::selectSlot(const std::string& selectedSlotId) {
	if (selectedSlotId.empty()) {
		deselectCurrentSlot();
		return;
	}
	
	if (selectedSlotId == m_selectedSlotId) return;

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
	if (!contains(m_items, m_selectedSlotId)) return nullptr;
	return &m_items.at(m_selectedSlotId);
}

void MerchantWindow::render(sf::RenderTarget& target) {
	m_window->render(target);
	target.draw(m_title);

	for (auto& it : m_items) {
		it.second.render(m_scrollHelper->texture);
	}
	m_scrollHelper->render(target);

	m_descriptionWindow->render(target);

	target.draw(m_scrollWindow);
	m_scrollBar->render(target);
}

void MerchantWindow::renderAfterForeground(sf::RenderTarget& target) {
	for (auto& it : m_items) {
		it.second.renderAfterForeground(target);
	}
}

void MerchantWindow::showDescription(const Item* item) {
	if (item == nullptr) return;
	m_descriptionWindow->setReputation(m_interface->getReputation(item), m_interface->isReputationReached(item));
	m_descriptionWindow->load(*item, m_interface->getCore(), m_interface->getMerchantData().multiplier, true);
	m_descriptionWindow->show();
	sf::Vector2f pos = sf::Vector2f(
		m_window->getPosition().x - WINDOW_MARGIN - m_descriptionWindow->getSize().x,
		m_window->getPosition().y + m_window->getSize().y - m_descriptionWindow->getSize().y);
	m_descriptionWindow->setPosition(pos);
}

void MerchantWindow::hideDescription() {
	m_descriptionWindow->hide();
}

void MerchantWindow::reload() {
	m_scrollBar->scroll(0);

	// reload items
	clearAllSlots();
	hideDescription();
	for (auto& it : m_interface->getMerchantData().wares) {
		if (!g_resourceManager->getItem(it.first)) {
			g_logger->logError("MerchantWindow", "Item not resolved: " + it.first);
			continue;
		}
		
		m_items.insert({ it.first, InventorySlot(it.first, it.second) });
	}
}

void MerchantWindow::calculateSlotPositions() {
	float number = static_cast<float>(m_items.size());
	int rows = static_cast<int>(std::ceil(number / SLOT_COUNT_X));
	int steps = rows - SLOT_COUNT_Y + 1;

	m_scrollBar->setDiscreteSteps(steps);

	int scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (scrollPos * (ICON_MARGIN + InventorySlot::SIZE) != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = scrollPos * (ICON_MARGIN + InventorySlot::SIZE);
	}

	float animationTime = 0.1f;
	float time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	float start = m_scrollHelper->lastOffset;
	float change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	float effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	float xOffsetStart = LEFT + SCROLL_WINDOW_LEFT + InventorySlot::ICON_OFFSET + 2 * WINDOW_MARGIN;

	float yOffset = TOP + SCROLL_WINDOW_TOP + InventorySlot::ICON_OFFSET + 2 * WINDOW_MARGIN - effectiveScrollOffset;
	float xOffset = xOffsetStart;
	int y = 1;
	int x = 1;
	for (auto& it : m_items) {
		it.second.setPosition(sf::Vector2f(xOffset, yOffset));
		if (x + 1 > SLOT_COUNT_X) {
			x = 1;
			xOffset = xOffsetStart;
			y++;
			yOffset += ICON_MARGIN + InventorySlot::SIZE;
		}
		else {
			x++;
			xOffset += ICON_MARGIN + InventorySlot::SIZE;
		}
	}
}
