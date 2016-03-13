#include "GUI/TexturedTabBar.h"

using namespace std;

TexturedTabBar::TexturedTabBar(const sf::FloatRect& box, int numberTabs) {
	m_tabButtons = std::vector<TexturedTabButton*>(numberTabs);
	float tabWidth = box.width / numberTabs + (numberTabs - 1) * TexturedTabButton::ALIGNMENT_OFFSET / numberTabs;
	for (int i = 0; i < numberTabs; ++i) {
		float x = box.left + i * (tabWidth - TexturedTabButton::ALIGNMENT_OFFSET);
		TexturedTabButton* tab = new TexturedTabButton(sf::FloatRect(x, box.top, tabWidth, box.height));
		m_tabButtons[i] = tab;
	}

	setBoundingBox(box);

	m_activeTabIndex = 0;
	m_tabButtons[0]->setActive(true);

	m_activeOverlay = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_tab_active), COLOR_WHITE, tabWidth, box.height);
	m_activeOverlay.setPosition(sf::Vector2f(box.left, box.top));
}

TexturedTabBar::~TexturedTabBar() {
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		delete m_tabButtons[i];
	}
}

void TexturedTabBar::render(sf::RenderTarget& renderTarget) {
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		m_tabButtons[i]->render(renderTarget);
	}
	renderTarget.draw(m_activeOverlay);
}

void TexturedTabBar::update(const sf::Time& frameTime) {
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
		int numberTabs = static_cast<int>(m_tabButtons.size());
		float tabWidth = bbox->width / numberTabs + (numberTabs - 1) * TabButton::ALIGNMENT_OFFSET / numberTabs;
		float x = bbox->left + m_activeTabIndex * (tabWidth - TabButton::ALIGNMENT_OFFSET);
		m_activeOverlay.setPosition(x, bbox->top);
	}

	GameObject::update(frameTime);
}

int TexturedTabBar::getActiveTabIndex() const {
	return m_activeTabIndex;
}

std::vector<TexturedTabButton*>& TexturedTabBar::getTabButtons() {
	return m_tabButtons;
}

TexturedTabButton* TexturedTabBar::getTabButton(int index) {
	return m_tabButtons[index];
}

GameObjectType TexturedTabBar::getConfiguredType() const {
	return GameObjectType::_Button;
}
