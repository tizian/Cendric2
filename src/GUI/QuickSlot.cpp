#include "GUI/QuickSlot.h"
#include "CharacterCore.h"
#include "Level/LevelInterface.h"
#include "Screens/LevelScreen.h"
#include "ResourceManager.h"
#include "GlobalResource.h"
#include "GameObjectComponents/TooltipWindowComponent.h"
#include "Controller/GamepadMappings.h"

const float QuickSlot::SIZE = 58.f;
const float QuickSlot::ICON_OFFSET = 4.f;

QuickSlot::QuickSlot(WorldInterface* _interface, Key key, const std::string& itemId) {
	m_interface = _interface;
	m_screen = _interface->getScreen();
	m_core = _interface->getCore();
	m_key = key;
	m_itemID = itemId;
	
	setBoundingBox(sf::FloatRect(0.f, 0.f, ICON_SIZE, ICON_SIZE));
	setDebugBoundingBox(COLOR_BAD);
	setInputInDefaultView(true);

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(COLOR_WHITE);

	m_backgroundRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_backgroundRect.setFillColor(COLOR_TRANS_GREY);

	m_overlayRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_overlayRect.setFillColor(sf::Color(0, 0, 0, 0));

	m_borderTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY);
	m_borderTextureSelected = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY_SELECTED);

	m_highlightTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_HIGHLIGHT);

	m_borderRect.setSize(sf::Vector2f(SIZE, SIZE));
	m_borderRect.setTexture(m_borderTexture);

	m_iconRect.setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_iconRect.setTexture(g_resourceManager->getTexture(GlobalResource::TEX_ITEMS));

	reload();
}

void QuickSlot::reloadKey() {
	std::string keyText;

	if (g_inputController->isGamepadConnected()) {
		keyText = GamepadMappings::getKeyName(m_key);
	} else {
		keyText = contains(g_resourceManager->getConfiguration().mainKeyMap, m_key) ?
			EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[m_key]) :
			"";
	}

	m_keyText.setString(keyText);
	m_keyText.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	if (m_keyText.getLocalBounds().width > SIZE - 10.f) {
		m_keyText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}
}

void QuickSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	
	sf::Vector2f positionOffset(ICON_SIZE / 2.f - m_keyText.getLocalBounds().width / 2.f, SIZE - ICON_OFFSET / 2.f);
	m_keyText.setPosition(pos + positionOffset);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + ICON_SIZE - m_amountText.getLocalBounds().width,
		pos.y + ICON_SIZE - m_amountText.getLocalBounds().height));
}

void QuickSlot::adjustTooltipOffset() {
	m_tooltipComponent->setWindowOffset(sf::Vector2f(-m_tooltipComponent->getWidth() + ICON_SIZE, -m_tooltipComponent->getHeight() - TOOLTIP_TOP));
}

void QuickSlot::setItemID(const std::string& itemID) {
	m_itemID = itemID;
	m_screen->notifyQuickSlotAssignment(itemID, m_key == Key::QuickSlot1 ? 1 : 2);
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
	if (LevelInterface* li = dynamic_cast<LevelInterface*>(m_interface)) {
		li->consumeItem(m_itemID);
	}
}

void QuickSlot::reload() {
	// check if item exists
	if (m_itemID.empty() || !contains(*m_core->getItems(), m_itemID)) {
		// the slot is empty
		m_isEmpty = true;
		m_iconRect.setTextureRect(sf::IntRect(0, 0, 0, 0));
		m_keyText.setColor(COLOR_GREY);
		m_amountText.setString("");
		m_itemID = "";
		m_tooltipComponent->setTooltipText("(" + g_textProvider->getText("Empty") + ")");
	}
	else {
		// the slot is filled
		m_isEmpty = false;
		Item* item = g_resourceManager->getItem(m_itemID);
		if (item == nullptr || item->getType() == ItemType::VOID) return;
		int amount = m_core->getItems()->at(m_itemID);

		m_iconRect.setTextureRect(sf::IntRect(
			item->getIconTextureLocation().x,
			item->getIconTextureLocation().y,
			static_cast<int>(ICON_SIZE),
			static_cast<int>(ICON_SIZE)));

		m_amountText.setString(std::to_string(amount));
		m_keyText.setColor(COLOR_WHITE);

		m_amountText.setPosition(sf::Vector2f(
			getPosition().x + ICON_SIZE - m_amountText.getLocalBounds().width,
			getPosition().y + ICON_SIZE - m_amountText.getLocalBounds().height));

		m_tooltipComponent->setMaxWidth(300);

		auto tooltip = g_textProvider->getText(m_itemID, "item") + "\n\n";
		AttributeData::appendAttributes(tooltip, item->getAttributes());
		m_tooltipComponent->setTooltipText(tooltip);
	}

	m_borderRect.setFillColor(m_isEmpty ? COLOR_MEDIUM_GREY : COLOR_WHITE);
	reloadKey();
	setPosition(getPosition());
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