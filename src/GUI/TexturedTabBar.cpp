#include "GUI/TexturedTabBar.h"
#include "GUI/GUIConstants.h"
#include "GlobalResource.h"

void TexturedTabBar::init(const sf::FloatRect& box, int numberTabs) {
	m_tabButtons = std::vector<TabButton*>(numberTabs);
	m_tabWidth = box.size.x / numberTabs + (numberTabs - 1) * TexturedTabButton::ALIGNMENT_OFFSET / numberTabs;
	for (int i = 0; i < numberTabs; ++i) {
		TexturedTabButton* tab = new TexturedTabButton(sf::FloatRect({0, 0}, {m_tabWidth, box.size.y}));
		m_tabButtons[i] = tab;
	}

	m_activeTabIndex = 0;
	m_tabButtons[0]->setActive(true);

	m_activeOverlay = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ACTIVE), COLOR_WHITE, m_tabWidth, box.size.y);

	m_leftText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_leftText.setColor(COLOR_BRIGHT_PURPLE);

	m_rightText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_rightText.setColor(COLOR_BRIGHT_PURPLE);

	setPosition({box.position.x, box.position.y});

	m_isGamepadEnabled = false;
}

TexturedTabButton* TexturedTabBar::getTabButton(int index) const {
	return dynamic_cast<TexturedTabButton*>(TabBar::getTabButton(index));
}

