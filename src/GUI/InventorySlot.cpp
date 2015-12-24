#include "GUI/InventorySlot.h"
#include "GUI/Inventory.h"

using namespace std;

const float InventorySlot::SIDE_LENGTH = 50.f;
const float InventorySlot::MARGIN = 2.f;

InventorySlot::InventorySlot() : m_item() {
	m_type = m_item.getType();

	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(CENDRIC_COLOR_WHITE);
	setAmount(0);

	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setOutlineColor(CENDRIC_COLOR_PURPLE);
}

InventorySlot::InventorySlot(const Item& item, int amount) : m_item(item.getID()) {
	m_type = m_item.getType();

	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_items));
	m_inside.setTextureRect(sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, static_cast<int>(SIDE_LENGTH), static_cast<int>(SIDE_LENGTH)));

	m_amountText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_amountText.setColor(CENDRIC_COLOR_WHITE);
	setAmount(amount);

	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setOutlineColor(CENDRIC_COLOR_PURPLE);
}

InventorySlot::InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos) : m_item() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_inside.setTexture(tex);
	m_inside.setTextureRect(sf::IntRect(texPos.x, texPos.y, static_cast<int>(SIDE_LENGTH), static_cast<int>(SIDE_LENGTH)));

	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setOutlineColor(CENDRIC_COLOR_DARK_GREY);
}

void InventorySlot::setAmount(int amount) {
	m_amountText.setString(amount < 0 ? "" : to_string(amount));
	setPosition(getPosition());
}

void InventorySlot::activate() {
	m_outside.setOutlineColor(sf::Color::Red);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_inside.setFillColor(sf::Color::White);
}

void InventorySlot::deactivate() {
	m_outside.setOutlineColor(CENDRIC_COLOR_BLACK);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_BLACK);
	m_inside.setFillColor(sf::Color(150, 150, 150));
}

void InventorySlot::highlight(bool highlight) {
	if (highlight) {
		m_outside.setOutlineColor(sf::Color::Green);
	}
	else {
		m_outside.setOutlineColor(m_isSelected ? sf::Color::Red : CENDRIC_COLOR_DARK_PURPLE);
	}
}

void InventorySlot::select() {
	if (m_isSelected || m_item.getType() == ItemType::VOID) return;
	m_isSelected = true;
	m_outside.setOutlineColor(sf::Color::Red);
}

void InventorySlot::deselect() {
	if (!m_isSelected || m_item.getType() == ItemType::VOID) return;
	m_isSelected = false;
	m_outside.setOutlineColor(CENDRIC_COLOR_DARK_PURPLE);
}

bool InventorySlot::isClicked() {
	if (m_item.getType() == ItemType::VOID) return false;
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

bool InventorySlot::isRightClicked() {
	if (m_item.getType() == ItemType::VOID) return false;
	bool wasRightClicked = m_isRightClicked;
	m_isRightClicked = false;
	return wasRightClicked;
}

void InventorySlot::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_inside.setPosition(pos);
	m_outside.setPosition(pos);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + SIDE_LENGTH - m_amountText.getLocalBounds().width,
		pos.y + SIDE_LENGTH - m_amountText.getLocalBounds().height));
}

void InventorySlot::setItemType(ItemType type) {
	m_type = type;
}

void InventorySlot::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_outside);
	renderTarget.draw(m_inside);
	renderTarget.draw(m_amountText);
}

void InventorySlot::onLeftJustPressed() {
	m_isClicked = true;
	g_inputController->lockAction();
}

void InventorySlot::onRightClick() {
	m_isRightClicked = true;
	g_inputController->lockAction();
}

GameObjectType InventorySlot::getConfiguredType() const {
	return GameObjectType::_Interface;
}

const Item& InventorySlot::getItem() const {
	return m_item;
}

ItemType InventorySlot::getItemType() const {
	return m_type;
}

const std::string& InventorySlot::getItemID() const {
	return m_item.getID();
}