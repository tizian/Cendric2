#include "GUI/InventorySlot.h"
#include "GUI/Inventory.h"

using namespace std;

const float InventorySlot::SIDE_LENGTH = 50.f;
const float InventorySlot::MARGIN = 2.f;

InventorySlot::InventorySlot(const Item& item, int amount) : m_item(item.getBean())
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);
	
	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_items));
	m_inside.setTextureRect(sf::IntRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, static_cast<int>(SIDE_LENGTH), static_cast<int>(SIDE_LENGTH)));

	m_amountText.setString(amount < 0 ? "" : to_string(amount));
	m_amountText.setCharacterSize(8);
	m_amountText.setColor(CENDRIC_COLOR_WHITE);

	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setOutlineColor(CENDRIC_COLOR_PURPLE);
}

InventorySlot::InventorySlot(const sf::Texture* tex, const sf::Vector2i& texPos) : m_item(DEFAULT_ITEM)
{
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

void InventorySlot::activate()
{
	m_outside.setOutlineColor(sf::Color::Red);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_inside.setFillColor(sf::Color::White);
}

void InventorySlot::deactivate()
{
	m_outside.setOutlineColor(CENDRIC_COLOR_BLACK);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_BLACK);
	m_inside.setFillColor(sf::Color(150, 150, 150));
}

void InventorySlot::select()
{
	if (m_isSelected || m_item.getType() == ItemType::VOID) return;
	m_isSelected = true;
	m_outside.setOutlineColor(sf::Color::Red);
}

void InventorySlot::deselect()
{
	if (!m_isSelected || m_item.getType() == ItemType::VOID) return;
	m_isSelected = false;
	m_outside.setOutlineColor(CENDRIC_COLOR_DARK_PURPLE);
}

bool InventorySlot::isClicked()
{
	if (m_item.getType() == ItemType::VOID) return false;
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

bool InventorySlot::isConsumed()
{
	if (m_item.getType() == ItemType::VOID) return false;
	bool wasConsumed = m_isConsumed;
	m_isConsumed = false;
	return wasConsumed;
}

void InventorySlot::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos);
	m_inside.setPosition(pos);
	m_outside.setPosition(pos);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + SIDE_LENGTH - m_amountText.getLocalBounds().width, 
		pos.y + SIDE_LENGTH - m_amountText.getLocalBounds().height));
}

void InventorySlot::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_outside);
	renderTarget.draw(m_inside);
	renderTarget.draw(m_amountText);
}

void InventorySlot::onLeftJustPressed()
{
	m_isClicked = true;
	g_inputController->lockAction();
}

void InventorySlot::onRightClick()
{
	if (m_item.getType() == ItemType::Consumable)
	{
		m_isConsumed = true;
		m_outside.setOutlineColor(sf::Color::Green);
		g_inputController->lockAction();
	}
}

GameObjectType InventorySlot::getConfiguredType() const
{
	return GameObjectType::_Interface;
}

const Item& InventorySlot::getItem() const
{
	return m_item;
}