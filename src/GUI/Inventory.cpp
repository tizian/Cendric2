#include "GUI/Inventory.h"

Inventory::Inventory(CharacterCore* core)
{
	m_core = core;

	// init window
	sf::FloatRect box(INVENTORY_LEFT, INVENTORY_TOP, INVENTORY_WIDTH, INVENTORY_HEIGHT);
	sf::Color mainColor = sf::Color(50, 50, 50, 100);
	sf::Color backColor = sf::Color::Transparent;
	m_window = new Window(box, WindowOrnamentStyle::LARGE, mainColor, backColor, CENDRIC_COLOR_PURPLE);

	// init text
	m_inventoryText = BitmapText("");
	m_inventoryText.setPosition(sf::Vector2f(INVENTORY_LEFT + TEXT_OFFSET, INVENTORY_TOP + TEXT_OFFSET));
	m_inventoryText.setColor(sf::Color::White);
	m_inventoryText.setCharacterSize(12);
}

Inventory::~Inventory()
{
	delete m_window;
}

void Inventory::update()
{
	if (m_isVisible)
	{
		// TODO: check whether an item is selected
	}
}

void Inventory::render(sf::RenderTarget& target) const
{
	if (m_isVisible)
	{
		m_window->render(target);
		target.draw(m_inventoryText);
	}
}

void Inventory::reload()
{
	// update text
	std::wstring inventoryText = L"";
	inventoryText.append(g_textProvider->getText("Inventory"));
	inventoryText.append(L"\n\n");
	inventoryText.append(g_textProvider->getText("Gold"));
	inventoryText.append(L": ");
	inventoryText.append(std::to_wstring(m_core->getData().gold));
	inventoryText.append(L"\n\n");
	// reload
	m_core->loadItems();
	for (auto it : m_core->getData().items)
	{
		inventoryText.append(g_textProvider->getText(m_core->getItem(it.first).getID()));
		inventoryText.append(L": ");
		inventoryText.append(std::to_wstring(it.second));
		inventoryText.append(L"\n");
	}
	m_inventoryText.setString(inventoryText);
}

void Inventory::show()
{
	m_isVisible = true;
	reload();
}

void Inventory::hide()
{
	m_isVisible = false;
}