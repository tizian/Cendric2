#include "GUI/LootWindow.h"
#include "ResourceManager.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(5.f, 10.f);

LootWindow::LootWindow(const sf::FloatRect& box) : Window(box, WindowOrnamentStyle::NONE, sf::Color(0, 0, 0, 70), sf::Color(0, 0, 0, 70), sf::Color::White)
{
}

LootWindow::~LootWindow()
{
	delete m_lootText;
}

void LootWindow::loadItemNames(const std::map<std::string, int>& loot, std::map<std::string, std::wstring>& names) const
{
	names.clear();
	for (auto &it : loot)
	{
		const ItemBean* bean = g_resourceManager->getItemBean(it.first);
		names.insert({ it.first, g_textProvider->getText(bean->id) });
	}
}

void LootWindow::setLoot(const std::map<string, int>& loot, int gold)
{
	delete m_lootText;
	wstring lootText = L"";
	lootText.append(g_textProvider->getText("Gold"));
	lootText.append(L": ");
	lootText.append(std::to_wstring(gold));
	lootText.append(L"\n\n");
	// reload
	std::map<std::string, std::wstring> names;
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