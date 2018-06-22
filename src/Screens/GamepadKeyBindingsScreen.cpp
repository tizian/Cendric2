#include "Screens/GamepadKeyBindingsScreen.h"
#include "Screens/OptionsScreen.h"
#include "Screens/MenuScreen.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GUI/Button.h"
#include "FileIO/ConfigurationWriter.h"
#include "GlobalResource.h"
#include "Controller/GamepadMappings/GamepadMappings.h"

const float GamepadKeyBindingsScreen::WINDOW_MARGIN = 6.f;

const float GamepadKeyBindingsScreen::WIDTH = 944.f;
const float GamepadKeyBindingsScreen::HEIGHT = 492.f;
const float GamepadKeyBindingsScreen::TOP = 75.f;
const float GamepadKeyBindingsScreen::LEFT = 0.5f * (WINDOW_WIDTH - WIDTH);

GamepadKeyBindingsScreen::GamepadKeyBindingsScreen(CharacterCore* core) : Screen(core) {
}

void GamepadKeyBindingsScreen::execUpdate(const sf::Time& frameTime) {
	if (m_lockInput) {
		g_inputController->lockAction();
		m_lockInput = false;
	}

	m_scrollBar->update(frameTime);

	if (m_selectedKey == Key::VOID && g_inputController->isKeyJustPressed(Key::Escape)) {
		setNextScreen(new OptionsScreen(m_characterCore));
		return;
	}

	if (m_selectedKey == Key::VOID && g_inputController->isKeyJustPressed(Key::Escape)) {
		reload();
	}
	else if (m_selectedKey != Key::VOID && g_inputController->getLastPressedGamepadInput() != GamepadInput::VOID) {
		if (!trySetKeyBinding(m_selectedKey, g_inputController->getLastPressedGamepadInput())) {
			setNegativeTooltip("KeyReserved");
		}
	}

	m_keyButtonGroup->update(frameTime);

	for (auto& it : m_keyButtons) {
		Button* keyButton = m_keyButtons[it.first].first;

		sf::Vector2f pos = keyButton->getPosition();
		if (pos.y < TOP) {
			if (!keyButton->isSelected()) {
				continue;
			}
			m_scrollBar->scroll(-1);
		}
		else if (pos.y + keyButton->getBoundingBox()->height > TOP + HEIGHT) {
			if (!keyButton->isSelected()) {
				continue;
			}
			m_scrollBar->scroll(1);
		}

		if (keyButton->isClicked()) {
			reload();
			keyButton->setText("PressAnyKey");
			m_selectedKey = it.first;
			break;
		}
	}

	updateObjects(_Button, frameTime);
	updateTooltipText(frameTime);

	calculateEntryPositions();
}

void GamepadKeyBindingsScreen::calculateEntryPositions() {
	float delta = 48.f;
	int ENTRY_COUNT = 10;

	int rows = static_cast<int>(m_keyButtons.size());
	int steps = rows - ENTRY_COUNT + 1;

	m_scrollBar->setDiscreteSteps(steps);

	int scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (scrollPos * delta != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = scrollPos * delta;
	}

	float animationTime = 0.1f;
	float time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	float start = m_scrollHelper->lastOffset;
	float change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	float effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	float yOffset = TOP + 2 * WINDOW_MARGIN - effectiveScrollOffset;
	float center = 0.5f * WINDOW_WIDTH;

	for (auto& it : m_keyButtons) {
		BitmapText* keyText = m_keyTexts[it.first];
		const sf::FloatRect& bbox = keyText->getBounds();
		keyText->setPosition(sf::Vector2f(center - 4.f * WINDOW_MARGIN - bbox.width, yOffset + 10.f));

		Button* keyButton = m_keyButtons[it.first].first;
		keyButton->setPosition(sf::Vector2f(center + 2.f * WINDOW_MARGIN, yOffset));

		yOffset += delta;
	}

	sf::Vector2f pos = m_keyButtonGroup->getSelectedButton()->getPosition();
	if (pos.y < TOP) {
		m_keyButtonGroup->setNextButtonSelectedY(true);
	}
	else if (pos.y + m_keyButtonGroup->getSelectedButton()->getBoundingBox()->height > TOP + HEIGHT) {
		m_keyButtonGroup->setNextButtonSelectedY(false);
	}
}

void GamepadKeyBindingsScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);

	for (auto it : m_keyTexts) {
		m_scrollHelper->texture.draw(*it.second);
	}
	m_keyButtonGroup->render(m_scrollHelper->texture);

	renderObjects(_Button, renderTarget);
	renderTooltipText(renderTarget);

	m_scrollHelper->render(renderTarget);

	renderTarget.draw(m_scrollWindow);
	m_scrollBar->render(renderTarget);
}

void GamepadKeyBindingsScreen::execOnEnter() {
	// title
	m_title = new BitmapText(g_textProvider->getText("Gamepad"), TextStyle::Shadowed);
	m_title->setCharacterSize(24);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 25.f));

	m_selectedKeys = g_resourceManager->getConfiguration().gamepadKeyMap;

	m_keyButtonGroup = new ButtonGroup();

	// init text and button objects once
	for (auto& it : m_selectedKeys) {
		BitmapText* keyText = new BitmapText(g_textProvider->getText(EnumNames::getKeyName(it.first)));
		keyText->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		m_keyTexts[it.first] = keyText;

		Button* keyButton = new Button(sf::FloatRect(0.f, 0.f, 150.f, 30.f));
		keyButton->setTextRaw(EnumNames::getGamepadInputName(it.second), 12);

		m_keyButtonGroup->addButton(keyButton);
		m_keyButtons[it.first] = std::pair<Button*, GamepadInput>(keyButton, it.second);
	}

	reload();

	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, WIDTH, HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT, TOP));

	m_scrollBar = new ScrollBar(HEIGHT);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + WIDTH - ScrollBar::WIDTH, TOP));

	sf::FloatRect scrollBox(LEFT, TOP, WIDTH, HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	const float buttonWidth = 240.f;
	const float buttonHeight = 50.f;
	const float marginX = 60.f;
	const float marginY = WINDOW_HEIGHT - 80.f;
	const float buttonSpaceWidth = WINDOW_WIDTH - 2 * marginX;
	const float buttonSpacing = (buttonSpaceWidth - 4 * buttonWidth) / 3.f;

	// back
	auto button = new Button(sf::FloatRect(marginX, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Back");
	button->setOnClick(std::bind(&GamepadKeyBindingsScreen::onBack, this));
	button->setGamepadKey(Key::Escape);
	addObject(button);
	// reset
	button = new Button(sf::FloatRect(marginX + buttonWidth + buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Reset");
	button->setOnClick(std::bind(&GamepadKeyBindingsScreen::onReset, this));
	button->setGamepadKey(Key::PreviousSpell);
	addObject(button);
	// default values
	button = new Button(sf::FloatRect(marginX + 2 * buttonWidth + 2 * buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Default");
	button->setGamepadKey(Key::NextSpell);
	button->setOnClick(std::bind(&GamepadKeyBindingsScreen::onUseDefault, this));
	addObject(button);
	// apply
	button = new Button(sf::FloatRect(marginX + 3 * buttonWidth + 3 * buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Apply");
	button->setGamepadKey(Key::Attack);
	button->setOnClick(std::bind(&GamepadKeyBindingsScreen::onApply, this));
	addObject(button);
}

bool GamepadKeyBindingsScreen::trySetKeyBinding(Key key, GamepadInput input) {
	for (auto& it : m_selectedKeys) {
		if (it.second == input) it.second = GamepadInput::MAX;
	}

	m_selectedKeys[key] = input;

	if (key == Key::Interact || key == Key::Confirm) {
		m_selectedKeys[Key::Interact] = input;
		m_selectedKeys[Key::Confirm] = input;
	}

	g_inputController->lockAction();
	m_lockInput = true;
	reload();
	return true;
}

void GamepadKeyBindingsScreen::reload() {
	float distFromTop = 120.f;

	// gamepad mappings
	for (auto& it : m_selectedKeys) {
		Button* keyButton = m_keyButtons[it.first].first;
		keyButton->setTextRaw(GamepadMappings::getKeyName(it.second), 12);

		if (it.second == GamepadInput::MAX) {
			keyButton->setTextColor(COLOR_BAD);
		}

		distFromTop = distFromTop + 40;
	}

	m_selectedKey = Key::VOID;
}


void GamepadKeyBindingsScreen::execOnExit() {
	// delete texts
	for (auto& it : m_keyTexts) {
		delete it.second;
	}
	// delete buttons
	delete m_keyButtonGroup;
	m_keyTexts.clear();
	m_keyButtons.clear();
	m_selectedKeys.clear();
	delete m_title;
	delete m_scrollBar;
	delete m_scrollHelper;
}

void GamepadKeyBindingsScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}

bool GamepadKeyBindingsScreen::checkSet(Key key) {
	if (m_selectedKeys[key] == GamepadInput::MAX) {
		setTooltipTextRaw(
			g_textProvider->getText(EnumNames::getKeyName(key)) + " " + 
			g_textProvider->getText("MustBeSet"), COLOR_GOOD, true);
		return false;
	}
	return true;
}

void GamepadKeyBindingsScreen::onApply() {
	if (!checkSet(Key::Confirm)) return;
	if (!checkSet(Key::Escape)) return;
	if (!checkSet(Key::Interact)) return;

	g_resourceManager->getConfiguration().gamepadKeyMap = m_selectedKeys;
	ConfigurationWriter writer;
	writer.saveToFile(g_resourceManager->getConfiguration());
	setTooltipText("ConfigurationSaved", COLOR_GOOD, true);
}

void GamepadKeyBindingsScreen::onUseDefault() {
	m_selectedKeys = GamepadMappings::getDefaultMappings(GamepadController::getCurrentGamepadProductId());
	reload();
}

void GamepadKeyBindingsScreen::onReset() {
	m_selectedKeys = g_resourceManager->getConfiguration().gamepadKeyMap;
	reload();
}