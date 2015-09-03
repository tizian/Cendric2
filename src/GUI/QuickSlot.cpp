#include "GUI/QuickSlot.h"
#include "CharacterCore.h"
#include "LevelInterface.h"

using namespace std;

const float QuickSlot::SIDE_LENGTH = 50.f;
const float QuickSlot::MARGIN = 2.f;

QuickSlot::QuickSlot(LevelInterface* _interface, const std::string& itemID, Key key) 
{
	m_interface = _interface;
	m_core = _interface->getCore();
	m_itemID = itemID;
	m_key = key;

	setBoundingBox(sf::FloatRect(0.f, 0.f, SIDE_LENGTH, SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	
	m_outside.setSize(sf::Vector2f(SIDE_LENGTH, SIDE_LENGTH));
	m_outside.setOutlineThickness(MARGIN);
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);

	m_amountText.setCharacterSize(8);
	m_amountText.setColor(CENDRIC_COLOR_WHITE);

	m_keyText.setString(key != Key::VOID ?
		EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[key]) :
		"");
	m_keyText.setCharacterSize(16);
	if (m_keyText.getLocalBounds().width > SIDE_LENGTH - 10.f) m_keyText.setCharacterSize(8);

	reload();
}

void QuickSlot::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos);
	m_inside.setPosition(pos);
	m_outside.setPosition(pos);
	m_keyText.setPosition(pos);
	m_amountText.setPosition(sf::Vector2f(
		pos.x + SIDE_LENGTH - m_amountText.getLocalBounds().width,
		pos.y + SIDE_LENGTH - m_amountText.getLocalBounds().height));
}

void QuickSlot::highlight(bool highlight)
{
	if (highlight)
	{
		m_outside.setOutlineColor(sf::Color::Green);
	}
	else
	{
		m_outside.setOutlineColor(m_isEmpty ? CENDRIC_COLOR_DARK_GREY : CENDRIC_COLOR_PURPLE);
	}
}

void QuickSlot::setItemID(const std::string& itemID)
{
	m_itemID = itemID;
	m_core->setQuickslot(itemID, m_key == Key::QuickSlot1 ? 1 : 2);
	reload();
}

void QuickSlot::update(const sf::Time& frameTime) 
{
	GameObject::update(frameTime);
	if (g_inputController->isKeyJustPressed(m_key))
	{
		consume();
	}
}

void QuickSlot::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_outside);
	renderTarget.draw(m_inside);
	renderTarget.draw(m_amountText);
	renderTarget.draw(m_keyText);
}

void QuickSlot::consume()
{
	if (m_isEmpty) return;
	m_interface->consumeItem(m_core->getItem(m_itemID));
}

void QuickSlot::reload()
{
	// check if item exists
	if (m_itemID.empty() || m_core->getItems()->find(m_itemID) == m_core->getItems()->end())
	{
		// the slot is empty
		m_isEmpty = true;
		m_inside.setFillColor(sf::Color::Transparent);
		m_outside.setOutlineColor(CENDRIC_COLOR_DARK_GREY);
		m_keyText.setColor(CENDRIC_COLOR_GREY);
		m_amountText.setString("");
		m_itemID = "";
	}
	else
	{
		// the slot is filled
		m_isEmpty = false;
		const Item& item = m_core->getItem(m_itemID);
		int amount = m_core->getItems()->at(m_itemID);

		m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_items));
		m_inside.setTextureRect(sf::IntRect(
			item.getIconTextureLocation().x, 
			item.getIconTextureLocation().y, 
			static_cast<int>(SIDE_LENGTH), 
			static_cast<int>(SIDE_LENGTH)));
		m_inside.setFillColor(sf::Color::White);

		m_outside.setOutlineColor(CENDRIC_COLOR_PURPLE);

		m_amountText.setString(to_string(amount));
		m_keyText.setColor(CENDRIC_COLOR_WHITE);
	}
}

void QuickSlot::onLeftJustPressed()
{
	consume();
	g_inputController->lockAction();
}

void QuickSlot::onRightClick()
{
	consume();
	g_inputController->lockAction();
}

GameObjectType QuickSlot::getConfiguredType() const
{
	return GameObjectType::_Interface;
}


