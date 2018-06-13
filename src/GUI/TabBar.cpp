#include "GUI/TabBar.h"
#include "GlobalResource.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"

TabBar::~TabBar() {
	CLEAR_VECTOR(m_tabButtons)
}

void TabBar::init(const sf::FloatRect& box, int numberTabs) {
	m_tabButtons = std::vector<TabButton*>(numberTabs);
	m_tabWidth = box.width / numberTabs + (numberTabs - 1) * TabButton::ALIGNMENT_OFFSET / numberTabs;
	for (auto i = 0; i < numberTabs; ++i) {
		const auto tab = new TabButton(sf::FloatRect(0, 0, m_tabWidth, box.height));
		m_tabButtons[i] = tab;
	}

	setBoundingBox(box);
	setDebugBoundingBox(COLOR_GOOD);

	m_activeTabIndex = 0;
	m_tabButtons[0]->setActive(true);

	m_activeOverlay = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ACTIVE), COLOR_WHITE, m_tabWidth, box.height);
	setPosition(sf::Vector2f(box.left, box.top));

	m_leftText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_leftText.setColor(COLOR_BRIGHT_PURPLE);

	m_rightText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_rightText.setColor(COLOR_BRIGHT_PURPLE);

	setGamepadEnabled(false);
}

void TabBar::loadLeftRightText() {
	if (!g_inputController->isSelected() || !m_isGamepadEnabled) {
		m_leftText.setString("");
		m_rightText.setString("");
		return;
	}

	auto const leftArrow = EnumNames::getArrowSymbolForKey(Key::Left);
	auto const leftText = EnumNames::getGamepadAxisName(g_resourceManager->getConfiguration().gamepadKeyMap[Key::PreviousSpell]);

	auto const rightArrow = EnumNames::getArrowSymbolForKey(Key::Right);
	auto const rightText = EnumNames::getGamepadAxisName(g_resourceManager->getConfiguration().gamepadKeyMap[Key::NextSpell]);

	m_leftText.setString(leftArrow + " " + leftText);
	m_rightText.setString(rightText + " " + rightArrow);

	setPosition(getPosition());
}

void TabBar::setPosition(const sf::Vector2f& position) {
	GameObject::setPosition(position);
	m_activeOverlay.setPosition(position);
	for (size_t i = 0; i < m_tabButtons.size(); ++i) {
		m_tabButtons[i]->setPosition(position + sf::Vector2f(i * (m_tabWidth - TabButton::ALIGNMENT_OFFSET), 0.f));
	}

	const float gamepadTextoffset = GUIConstants::CHARACTER_SIZE_M + 10.f;

	m_leftText.setPosition(position + sf::Vector2f(0, -gamepadTextoffset));

	if (m_tabButtons.empty())
	{
		return;
	}

	auto const tabRight = m_tabButtons[static_cast<int>(m_tabButtons.size()) - 1]->getPosition() + sf::Vector2f(m_tabWidth, -gamepadTextoffset);
	m_rightText.setPosition(tabRight - sf::Vector2f(m_rightText.getLocalBounds().width, 0));
}

void TabBar::render(sf::RenderTarget& renderTarget) {
	for (auto& tabButton : m_tabButtons) {
		tabButton->render(renderTarget);
	}
	renderTarget.draw(m_activeOverlay);
	renderTarget.draw(m_leftText);
	renderTarget.draw(m_rightText);
}

void TabBar::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	for (auto& tabButton : m_tabButtons) {
		tabButton->renderAfterForeground(renderTarget);
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

	if (nextActiveIndex == -1 && m_isGamepadEnabled) {
		if (g_inputController->isKeyJustPressed(Key::NextSpell)) {
			nextActiveIndex = std::min(static_cast<int>(m_tabButtons.size() - 1), m_activeTabIndex + 1);
			if (nextActiveIndex != m_activeTabIndex) {
				m_tabButtons[nextActiveIndex]->onLeftJustPressed();
				m_tabButtons[nextActiveIndex]->onLeftClick();
			}
		}
		else if (g_inputController->isKeyJustPressed(Key::PreviousSpell)) {
			nextActiveIndex = std::max(0, m_activeTabIndex - 1);
			if (nextActiveIndex != m_activeTabIndex) {
				m_tabButtons[nextActiveIndex]->onLeftJustPressed();
				m_tabButtons[nextActiveIndex]->onLeftClick();
			}
		}
	}

	if (nextActiveIndex != -1) {
		for (auto& button : m_tabButtons) {
			button->setActive(false);
		}
		m_activeTabIndex = nextActiveIndex;
		m_tabButtons[m_activeTabIndex]->setActive(true);

		auto const bbox = getBoundingBox();
		auto const x = bbox->left + m_activeTabIndex * (m_tabWidth - TabButton::ALIGNMENT_OFFSET);
		m_activeOverlay.setPosition(x, bbox->top);
	}

	GameObject::update(frameTime);
}

void TabBar::setGamepadEnabled(bool enabled) {
	m_isGamepadEnabled = enabled;
	loadLeftRightText();
}

int TabBar::getActiveTabIndex() const {
	return m_activeTabIndex;
}

std::vector<TabButton*>& TabBar::getTabButtons() {
	return m_tabButtons;
}

TabButton* TabBar::getTabButton(int index) const {
	if (index < 0 || index > static_cast<int>(m_tabButtons.size()) - 1) {
		return nullptr;
	}

	return m_tabButtons.at(index);
}

GameObjectType TabBar::getConfiguredType() const {
	return _Button;
}
