#include "GUI/TexturedTabBar.h"
#include "GlobalResource.h"

void TexturedTabBar::init(const sf::FloatRect& box, int numberTabs) {
	m_tabButtons = std::vector<TabButton*>(numberTabs);
	m_tabWidth = box.width / numberTabs + (numberTabs - 1) * TexturedTabButton::ALIGNMENT_OFFSET / numberTabs;
	for (int i = 0; i < numberTabs; ++i) {
		TexturedTabButton* tab = new TexturedTabButton(sf::FloatRect(0, 0, m_tabWidth, box.height));
		m_tabButtons[i] = tab;
	}

	m_activeTabIndex = 0;
	m_tabButtons[0]->setActive(true);

	m_activeOverlay = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ACTIVE), COLOR_WHITE, m_tabWidth, box.height);

	setPosition(sf::Vector2f(box.left, box.top));

	m_isGamepadEnabled = false;
}

TexturedTabButton* TexturedTabBar::getTabButton(int index) const {
	return dynamic_cast<TexturedTabButton*>(TabBar::getTabButton(index));
}

