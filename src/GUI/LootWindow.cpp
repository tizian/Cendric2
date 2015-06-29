#include "GUI/LootWindow.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(5.f, 10.f);

LootWindow::LootWindow(const sf::FloatRect& box) : Window(box, WindowOrnamentStyle::SMALL, sf::Color(0, 0, 0, 100), sf::Color(0, 0, 0, 100), sf::Color::White)
{
}

LootWindow::~LootWindow()
{
	delete m_lootText;
}

void LootWindow::loadItemNames(const std::map<ItemID, int>& loot, std::map<ItemID, std::wstring>& names) const
{
	names.clear();
	ItemBean newItem;
	ItemFactory factory;
	for (auto &it : loot)
	{
		newItem = DEFAULT_ITEM;
		factory.loadItemBean(newItem, it.first);
		names.insert({ it.first, g_textProvider->getText(Item(newItem).getName()) });
	}
}

void LootWindow::setLoot(const std::map<ItemID, int>& loot, int gold)
{
	delete m_lootText;
	wstring lootText = L"";
	lootText.append(g_textProvider->getText("Gold"));
	lootText.append(L": ");
	lootText.append(std::to_wstring(gold));
	lootText.append(L"\n\n");
	// reload
	std::map<ItemID, std::wstring> names;
	loadItemNames(loot, names);
	for (auto &it : loot)
	{
		lootText.append(names.at(it.first));
		lootText.append(L": ");
		lootText.append(std::to_wstring(it.second));
		lootText.append(L"\n");
	}
	
	m_lootText = new BitmapText(lootText);
	m_lootText->setCharacterSize(8);
	m_lootText->setPosition(getPosition() + TEXT_OFFSET);
}

void LootWindow::setPosition(const sf::Vector2f& position)
{
	Window::setPosition(position);
	if (m_lootText != nullptr)
	{
		m_lootText->setPosition(position + TEXT_OFFSET);
	}
}

void LootWindow::render(sf::RenderTarget& renderTarget)
{
	Window::render(renderTarget);
	if (m_lootText != nullptr)
	{
		renderTarget.draw(*m_lootText);
	}
}