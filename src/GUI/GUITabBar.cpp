#include "GUI/GUITabBar.h"

#include "GUI/WorldInterface.h"
#include "GUI/GUIConstants.h"
#include "GUI/GUITabButton.h"
#include "GUI/ButtonGroup.h"

const int GUITabBar::BUTTON_MARGIN = 24;
const int GUITabBar::WIDTH = 80;

GUITabBar::GUITabBar(WorldInterface* _interface, int size) {
	m_worldInterface = _interface;
	m_size = std::max(0, size);
	init();
}

GUITabBar::~GUITabBar() {
	delete m_window;
	delete m_buttonGroup;
}

void GUITabBar::init() {
	float height = m_size * GUITabButton::SIZE + (m_size - 1) * BUTTON_MARGIN + 4 * GUIConstants::TEXT_OFFSET;
	
	sf::FloatRect box(0, 0, static_cast<float>(WIDTH), height);
	m_window = new Window(box, GUIOrnamentStyle::LARGE, GUIConstants::MAIN_COLOR, GUIConstants::ORNAMENT_COLOR);
	
	m_buttonGroup = new ButtonGroup();
	m_buttonGroup->setSelectableWindow(this);

	for (auto i = 0; i < m_size; ++i) {
		auto const button = new GUITabButton();
		m_buttonGroup->addButton(button);
	}

	setPosition(sf::Vector2f(0.f, 0.f));
}

void GUITabBar::setPosition(const sf::Vector2f& pos) {
	m_window->setPosition(pos);

	sf::Vector2f buttonPos;
	buttonPos.x = pos.x + 0.5f * (WIDTH - GUITabButton::SIZE);
	buttonPos.y = pos.y + 2 * GUIConstants::TEXT_OFFSET;
	for (auto button : m_buttonGroup->getButtons()) {
		button->setPosition(buttonPos);
		buttonPos.y += GUITabButton::SIZE + BUTTON_MARGIN;
	}
}

void GUITabBar::setButtonTexture(int index, const sf::Texture* tex, int x) {
	auto button = dynamic_cast<GUITabButton*>(m_buttonGroup->getButton(index));
	if (!button) return;

	button->setTexture(tex, x);
}

void GUITabBar::setButtonText(int index, const std::string& text) {
	auto button = dynamic_cast<GUITabButton*>(m_buttonGroup->getButton(index));
	if (!button) return;

	button->setText(text);
}

void GUITabBar::setButtonOnClick(int index, const std::function<void()>& agent) {
	auto button = dynamic_cast<GUITabButton*>(m_buttonGroup->getButton(index));
	if (!button) return;

	button->setOnClick(agent);
}

void GUITabBar::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	if (!getInterface()->isGuiOverlayVisible()) {
		hide();
	}

	auto* previousButton = m_buttonGroup->getSelectedButton();
	m_buttonGroup->update(frameTime);
	auto* currentButton = m_buttonGroup->getSelectedButton();

	auto nextSelectedIndex = -1;
	for (auto i = 0; i < m_buttonGroup->getButtons().size(); ++i) {
		auto tab = dynamic_cast<GUITabButton*>(m_buttonGroup->getButton(i));
		if (tab->isClicked() && !tab->isSelected()) {
			nextSelectedIndex = static_cast<int>(i);
		}
	}

	if (nextSelectedIndex != -1) {
		for (auto& button : m_buttonGroup->getButtons()) {
			button->setSelected(false);
		}
		m_buttonGroup->getButton(nextSelectedIndex)->setSelected(true);
	} else if (currentButton != previousButton) {
		currentButton->click();
	}

	currentButton = m_buttonGroup->getSelectedButton();
	if (currentButton != previousButton) {
		updateGamepadTexts();
	}
}

void GUITabBar::updateGamepadTexts() {
	if (!g_inputController->isGamepadConnected()) {
		return;
	}

	const auto selectedId = m_buttonGroup->getSelectedButtonId();
	const auto size = static_cast<int>(m_buttonGroup->getButtons().size());

	for (auto i = 0; i < size; ++i) {
		if (i == selectedId - 1) {
			setButtonText(i, EnumNames::getArrowSymbolForKey(Key::Down));
		}
		else if (i == selectedId && selectedId != size - 1) {
			setButtonText(i, EnumNames::getArrowSymbolForKey(Key::Up));
		}
		else {
			setButtonText(i, "");
		}
	}
}

void GUITabBar::setSelectedElement(int index) {
	if (index < 0 || index > static_cast<int>(m_buttonGroup->getButtons().size()) - 1) return;

	for (auto& button : m_buttonGroup->getButtons()) {
		button->setSelected(false);
	}
	m_buttonGroup->getButton(index)->setSelected(true);
	updateGamepadTexts();
}

int GUITabBar::getSelectedElement() const {
	for (auto i = 0; i < m_buttonGroup->getButtons().size(); ++i) {
		if (m_buttonGroup->getButton(i)->isSelected()) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

void GUITabBar::updateWindowSelected() {
	m_buttonGroup->setGamepadEnabled(isWindowSelected());
}

void GUITabBar::render(sf::RenderTarget& target) const {
	if (!m_isVisible) return;

	m_window->render(target);
	m_buttonGroup->render(target);
}

void GUITabBar::show(int index) {
	if (index < 0 || index > static_cast<int>(m_buttonGroup->getButtons().size()) - 1) return;

	m_buttonGroup->selectButton(index);
	updateGamepadTexts();
	show();
}

void GUITabBar::show() {
	m_isVisible = true;
}

void GUITabBar::hide() {
	m_isVisible = false;
}

bool GUITabBar::isVisible() const {
	return m_isVisible;
}

WorldInterface* GUITabBar::getInterface() const {
	return m_worldInterface;
}