#include "GUI/LootWindow.h"
#include "ResourceManager.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(5.f, 10.f);

LootWindow::LootWindow() : Window(sf::FloatRect(), GUIOrnamentStyle::NONE, sf::Color(0, 0, 0, 70), COLOR_WHITE) {
}

LootWindow::~LootWindow() {
	delete m_lootText;
}

void LootWindow::loadItemNames(const std::map<std::string, int>& loot, std::map<std::string, std::string>& names) const {
	names.clear();
	for (auto &it : loot) {
		ItemBean bean = g_databaseManager->getItemBean(it.first);
		names.insert({ it.first, g_textProvider->getText(bean.item_id, "item") });
	}
}

void LootWindow::setLoot(const std::map<string, int>& loot, int gold) {
	delete m_lootText;
	string lootText = "";
	// reload
	std::map<std::string, std::string> names;
	loadItemNames(loot, names);
	for (auto &it : loot) {
		if (!lootText.empty()) lootText.append("\n");
		lootText.append(names.at(it.first));
		lootText.append(": ");
		lootText.append(std::to_string(it.second));
	}
	if (gold > 0) {
		if (!lootText.empty()) lootText.append("\n\n");
		lootText.append(g_textProvider->getText("Gold"));
		lootText.append(": ");
		lootText.append(std::to_string(gold));
	}

	m_lootText = new BitmapText(lootText);
	m_lootText->setCharacterSize(8);
	m_lootText->setPosition(getPosition() + TEXT_OFFSET);
	setHeight(m_lootText->getLocalBounds().height + 18.f);
	setWidth(m_lootText->getLocalBounds().width + 10.f);
}

void LootWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	if (m_lootText != nullptr) {
		m_lootText->setPosition(position + TEXT_OFFSET);
	}
}

void LootWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	if (m_lootText != nullptr) {
		renderTarget.draw(*m_lootText);
	}
}