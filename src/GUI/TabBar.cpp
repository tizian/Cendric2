#include "GUI/TabBar.h"

using namespace std;

TabBar::TabBar(const sf::FloatRect& box, int numberTabs) {
	m_tabButtons = std::vector<TabButton*>(numberTabs);
	float tabWidth = box.width / numberTabs + (numberTabs - 1) * TabButton::ALIGNMENT_OFFSET / numberTabs;
	for (int i = 0; i < numberTabs; ++i) {
		float x = box.left + i * (tabWidth - TabButton::ALIGNMENT_OFFSET);
		TabButton* tab = new TabButton(sf::FloatRect(x, box.top, tabWidth, box.height));
		m_tabButtons[i] = tab;
	}

	setBoundingBox(box);

	m_activeTabIndex = 0;
	m_tabButtons[0]->setActive(true);

	m_activeOverlay = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_tab_active), CENDRIC_COLOR_WHITE, tabWidth, box.height);
	m_activeOverlay.setPosition(sf::Vector2f(box.left, box.top));
}

TabBar::~TabBar() {
	for (int i = 0; i < m_tabButtons.size(); ++i) {
		delete m_tabButtons[i];
	}
}

void TabBar::render(sf::RenderTarget& renderTarget) {
	for (int i = 0; i < m_tabButtons.size(); ++i) {
		m_tabButtons[i]->render(renderTarget);
	}
	renderTarget.draw(m_activeOverlay);
}

void TabBar::update(const sf::Time& frameTime) {
	int nextActiveIndex = -1;
	for (int i = 0; i < m_tabButtons.size(); ++i) {
		TabButton* tab = m_tabButtons[i];
		tab->update(frameTime);
		if (tab->isClicked() && !tab->isActive()) {
			nextActiveIndex = i;
		}
	}

	if (nextActiveIndex != -1) {
		for (int i = 0; i < m_tabButtons.size(); ++i) {
			m_tabButtons[i]->setActive(false);
		}
		m_activeTabIndex = nextActiveIndex;
		m_tabButtons[m_activeTabIndex]->setActive(true);

		const sf::FloatRect* bbox = getBoundingBox();
		int numberTabs = m_tabButtons.size();
		float tabWidth = bbox->width / numberTabs + (numberTabs - 1) * TabButton::ALIGNMENT_OFFSET / numberTabs;
		float x = bbox->left + m_activeTabIndex * (tabWidth - TabButton::ALIGNMENT_OFFSET);
		m_activeOverlay.setPosition(x, bbox->top);
	}

	GameObject::update(frameTime);
}

int TabBar::getActiveTabIndex() const {
	return m_activeTabIndex;
}

std::vector<TabButton*>& TabBar::getTabButtons() {
	return m_tabButtons;
}

TabButton* TabBar::getTabButton(int index) {
	return m_tabButtons[index];
}

GameObjectType TabBar::getConfiguredType() const {
	return GameObjectType::_Button;
}