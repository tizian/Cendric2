#include "GUI/TabBar.h"
#include "GlobalResource.h"

using namespace std;

TabBar::TabBar(const sf::FloatRect& box, int numberTabs) {
	m_tabButtons = std::vector<TabButton*>(numberTabs);
	m_tabWidth = box.width / numberTabs + (numberTabs - 1) * TabButton::ALIGNMENT_OFFSET / numberTabs;
	for (int i = 0; i < numberTabs; ++i) {
		TabButton* tab = new TabButton(sf::FloatRect(0, 0, m_tabWidth, box.height));
		m_tabButtons[i] = tab;
	}

	setBoundingBox(box);
	setDebugBoundingBox(COLOR_GOOD);

	m_activeTabIndex = 0;
	m_tabButtons[0]->setActive(true);

	m_activeOverlay = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ACTIVE), COLOR_WHITE, m_tabWidth, box.height);
	setPosition(sf::Vector2f(box.left, box.top));
}

TabBar::~TabBar() {
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		delete m_tabButtons[i];
	}
}

void TabBar::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	m_activeOverlay.setPosition(position);
	for (int i = 0; i < m_tabButtons.size(); ++i) {
		m_tabButtons[i]->setPosition(position + sf::Vector2f(i * (m_tabWidth - TabButton::ALIGNMENT_OFFSET), 0.f));
	}
}

void TabBar::render(sf::RenderTarget& renderTarget) {
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		m_tabButtons[i]->render(renderTarget);
	}
	renderTarget.draw(m_activeOverlay);
}

void TabBar::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		m_tabButtons[i]->renderAfterForeground(renderTarget);
	}
}

void TabBar::update(const sf::Time& frameTime) {
	int nextActiveIndex = -1;
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		TabButton* tab = m_tabButtons[i];
		tab->update(frameTime);
		if (tab->isClicked() && !tab->isActive()) {
			nextActiveIndex = static_cast<int>(i);
		}
	}

	if (nextActiveIndex != -1) {
		for (size_t i = 0; i < m_tabButtons.size(); ++i) {
			m_tabButtons[i]->setActive(false);
		}
		m_activeTabIndex = nextActiveIndex;
		m_tabButtons[m_activeTabIndex]->setActive(true);

		const sf::FloatRect* bbox = getBoundingBox();
		float x = bbox->left + m_activeTabIndex * (m_tabWidth - TabButton::ALIGNMENT_OFFSET);
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