#include "GUI/QuickSlot.h"
#include "CharacterCore.h"
#include "Level/LevelInterface.h"
#include "ResourceManager.h"

using namespace std;

const float QuickSlot::SIZE = 58.f;
const float QuickSlot::ICON_OFFSET = 4.f;

QuickSlot::QuickSlot(LevelInterface* _interface, const std::string& itemID, Key key) {
	m_interface = _interface;
	m_core = _interface->getCore();
	m_itemID = itemID;
	m_key = key;

	setBoundingBox(sf::FloatRect(0.f, 0.f, SIZE, SIZE));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_amountText.setCharacterSize(8);
	m_amountText.setColor(CENDRIC_COLOR_WHITE);

	m_keyText.setString(key != Key::VOID ?
		EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[key]) :
		"");
	m_keyText.setCharacterSize(16);
	if (m_keyText.getLocalBounds().width > SIZE - 10.f) m_keyText.setCharacterSize(8);

	m_backgroundRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_backgroundRect.setFillColor(CENDRIC_COLOR_TRANS_GREY);

	m_overlayRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 0));

	m_borderTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_inventory);
	m_borderTextureSelected = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_inventory_selected);

	m_borderRect.setSize(sf::Vector2f(SIZE, SIZE));
	m_borderRect.setTexture(m_borderTexture);

	m_iconRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_iconRect.setTexture(g_resourceManager->getTexture(ResourceID::Texture_items));

	reload();
}

void QuickSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	m_keyText.setPosition(pos);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + SIZE - m_amountText.getLocalBounds().width,
		pos.y + SIZE - m_amountText.getLocalBounds().height));
}

void QuickSlot::setItemID(const std::string& itemID) {
	m_itemID = itemID;
	m_core->setQuickslot(itemID, m_key == Key::QuickSlot1 ? 1 : 2);
	reload();
	g_inputController->lockAction();
}

void QuickSlot::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(m_key)) {
		consume();
		g_inputController->lockAction();
	}
	Slot::update(frameTime);
}

void QuickSlot::render(sf::RenderTarget& renderTarget) {
	Slot::render(renderTarget);
	renderTarget.draw(m_amountText);
	renderTarget.draw(m_keyText);
}

void QuickSlot::consume() {
	if (m_isEmpty) return;
	const Item* item = m_core->getItem(m_itemID);
	if (item == nullptr) return;
	m_interface->consumeItem(*item);
}

void QuickSlot::reload() {
	// check if item exists
	if (m_itemID.empty() || m_core->getItems()->find(m_itemID) == m_core->getItems()->end()) {
		// the slot is empty
		m_isEmpty = true;
		m_iconRect.setTextureRect(sf::IntRect(0, 0, 0, 0));
		m_keyText.setColor(CENDRIC_COLOR_GREY);
		m_amountText.setString("");
		m_itemID = "";
	}
	else {
		// the slot is filled
		m_isEmpty = false;
		const Item* item = m_core->getItem(m_itemID);
		if (item == nullptr) return;
		int amount = m_core->getItems()->at(m_itemID);

		m_iconRect.setTextureRect(sf::IntRect(
			item->getIconTextureLocation().x,
			item->getIconTextureLocation().y,
			static_cast<int>(ICON_SIZE),
			static_cast<int>(ICON_SIZE)));

		m_amountText.setString(to_string(amount));
		m_keyText.setColor(CENDRIC_COLOR_WHITE);

		m_amountText.setPosition(sf::Vector2f(
			getPosition().x + SIZE - m_amountText.getLocalBounds().width,
			getPosition().y + SIZE - m_amountText.getLocalBounds().height));
	}
}

void QuickSlot::onLeftClick() {
	consume();
	Slot::onLeftClick();
	g_inputController->lockAction();
}

void QuickSlot::onRightClick() {
	setItemID("");
	Slot::onRightClick();
	g_inputController->lockAction();
}