#include "Screens/KeyBindingsScreen.h"
#include "Screens/OptionsScreen.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"

const float KeyBindingsScreen::WINDOW_MARGIN = 6.f;

const float KeyBindingsScreen::WIDTH = 944.f;	
const float KeyBindingsScreen::HEIGHT = 492.f;
const float KeyBindingsScreen::TOP = 75.f;
const float KeyBindingsScreen::LEFT = 0.5f * (WINDOW_WIDTH - WIDTH);

const std::set<Key> KeyBindingsScreen::UNMODIFIABLE_KEYS = {
	Key::Escape,
	Key::Confirm
};

const std::set<Key> KeyBindingsScreen::INVISIBLE_KEYS = {
	Key::Debug
};

const std::set<sf::Keyboard::Key> KeyBindingsScreen::RESERVED_KEYS = {
	sf::Keyboard::Escape,
	sf::Keyboard::Numpad1,
	sf::Keyboard::Numpad2,
	sf::Keyboard::Numpad3,
	sf::Keyboard::Numpad4,
	sf::Keyboard::Numpad5,
	sf::Keyboard::Numpad6,
	sf::Keyboard::Numpad7,
	sf::Keyboard::Return,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Left,
	sf::Keyboard::Right,
};

KeyBindingsScreen::KeyBindingsScreen(CharacterCore* core) : Screen(core) {
}

void KeyBindingsScreen::execUpdate(const sf::Time& frameTime) {
	m_scrollBar->update(frameTime);

	if (m_selectedKey == Key::VOID && g_inputController->isKeyJustPressed(Key::Escape)) {
		setNextScreen(new OptionsScreen(m_characterCore));
		return;
	}
	else if (m_selectedKey == Key::VOID && g_inputController->isKeyJustPressed(Key::Escape)) {
		reload();
	}
	else if (m_selectedKey != Key::VOID && g_inputController->getLastPressedKey() != sf::Keyboard::Unknown) {
		if (!trySetKeyBinding(m_selectedKey, g_inputController->getLastPressedKey())) {
			setTooltipText("KeyReserved", COLOR_BAD, true);
		}
	}

	for (auto& it : m_keyButtons) {
		Button* keyButton = m_keyButtons[it.first].first;
		sf::Vector2f pos = keyButton->getPosition();
		if (pos.y < TOP || pos.y + keyButton->getBoundingBox()->height > TOP + HEIGHT) continue;

		keyButton->update(frameTime);

		if (keyButton->isClicked()) {
			keyButton->setText("PressAnyKey");
			m_selectedKey = it.first;
		}
	}

	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);

	calculateEntryPositions();
}

void KeyBindingsScreen::calculateEntryPositions() {
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
		sf::FloatRect& bbox = keyText->getBounds();
		keyText->setPosition(sf::Vector2f(center - 4.f * WINDOW_MARGIN - bbox.width, yOffset + 10.f));

		Button* keyButton = m_keyButtons[it.first].first;
		keyButton->setPosition(sf::Vector2f(center + 2.f * WINDOW_MARGIN, yOffset));

		yOffset += delta;
	}
}

void KeyBindingsScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);

	for (auto it : m_keyTexts) {
		m_scrollHelper->texture.draw(*it.second);
	}
	for (auto it : m_keyButtons) {
		it.second.first->render(m_scrollHelper->texture);
	}

	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);

	m_scrollHelper->render(renderTarget);

	renderTarget.draw(m_scrollWindow);
	m_scrollBar->render(renderTarget);
}

void KeyBindingsScreen::execOnEnter(const Screen *previousScreen) {
	// title
	m_title = new BitmapText(g_textProvider->getText("KeyBindings"));
	m_title->setCharacterSize(24);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 25.f));

	m_selectedKeys = g_resourceManager->getConfiguration().mainKeyMap;

	// init text and button objects once
	for (auto& it : m_selectedKeys) {
		if (INVISIBLE_KEYS.find(it.first) != INVISIBLE_KEYS.end()) continue;
		BitmapText* keyText = new BitmapText(g_textProvider->getText(EnumNames::getKeyName(it.first)));
		keyText->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		m_keyTexts[it.first] = keyText;

		Button* keyButton = new Button(sf::FloatRect(0.f, 0.f, 150.f, 30.f));
		keyButton->setTextRaw(EnumNames::getKeyboardKeyName(it.second), 12);
		m_keyButtons[it.first] = std::pair<Button*, sf::Keyboard::Key>(keyButton, it.second);
	}

	reload();

	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), COLOR_WHITE, WIDTH, HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT, TOP));

	m_scrollBar = new ScrollBar(HEIGHT);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + WIDTH - ScrollBar::WIDTH, TOP));

	sf::FloatRect scrollBox(LEFT, TOP, WIDTH, HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	const float buttonWidth = 200.f;
	const float buttonHeight = 50.f;
	const float marginX = 60.f;
	const float marginY = WINDOW_HEIGHT - 80.f;
	const float buttonSpaceWidth = WINDOW_WIDTH - 2 * marginX;
	const float buttonSpacing = (buttonSpaceWidth - 4 * buttonWidth) / 3.f;

	Button* button;
	// back
	button = new Button(sf::FloatRect(marginX, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Back");
	button->setOnClick(std::bind(&KeyBindingsScreen::onBack, this));
	addObject(button);
	// reset
	button = new Button(sf::FloatRect(marginX + buttonWidth + buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Reset");
	button->setOnClick(std::bind(&KeyBindingsScreen::onReset, this));
	addObject(button);
	// default values
	button = new Button(sf::FloatRect(marginX + 2 * buttonWidth + 2 * buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Default");
	button->setOnClick(std::bind(&KeyBindingsScreen::onUseDefault, this));
	addObject(button);
	// apply
	button = new Button(sf::FloatRect(marginX + 3 * buttonWidth + 3 * buttonSpacing, marginY, buttonWidth, buttonHeight), GUIOrnamentStyle::SMALL);
	button->setText("Apply");
	button->setOnClick(std::bind(&KeyBindingsScreen::onApply, this));
	addObject(button);
}

bool KeyBindingsScreen::trySetKeyBinding(Key key, sf::Keyboard::Key keyboardKey) {
	if (RESERVED_KEYS.find(keyboardKey) != RESERVED_KEYS.end()) {
		reload();
		return false;
	}

	for (auto& it : m_selectedKeys) {
		if (it.second == keyboardKey) it.second = sf::Keyboard::KeyCount;
	}
	m_selectedKeys[key] = keyboardKey;
	reload();
	return true;
}

void KeyBindingsScreen::reload() {
	float distFromTop = 120.f;
	float distFromLeft = 150.f;

	// keyboard mappings
	for (auto& it : m_selectedKeys) {
		if (INVISIBLE_KEYS.find(it.first) != INVISIBLE_KEYS.end()) continue;

		Button* keyButton = m_keyButtons[it.first].first;
		keyButton->setTextRaw(EnumNames::getKeyboardKeyName(it.second), 12);

		if (it.second == sf::Keyboard::KeyCount) {
			keyButton->setTextColor(COLOR_BAD);
		}
		if (UNMODIFIABLE_KEYS.find(it.first) != UNMODIFIABLE_KEYS.end()) {
			keyButton->setEnabled(false);
		}

		distFromTop = distFromTop + 40;
	}

	m_selectedKey = Key::VOID;
}


void KeyBindingsScreen::execOnExit(const Screen *nextScreen) {
	// delete texts
	for (auto& it : m_keyTexts) {
		delete it.second;
	}
	// delete buttons
	for (auto it : m_keyButtons) {
		delete it.second.first;
	}
	m_keyTexts.clear();
	m_keyButtons.clear();
	m_selectedKeys.clear();
	delete m_title;
	delete m_scrollBar;
	delete m_scrollHelper;
}

void KeyBindingsScreen::onBack() {
	setNextScreen(new OptionsScreen(m_characterCore));
}

void KeyBindingsScreen::onApply() {
	g_resourceManager->getConfiguration().mainKeyMap = m_selectedKeys;
	ConfigurationWriter writer;
	writer.saveToFile(g_resourceManager->getConfiguration());
	setTooltipText("ConfigurationSaved", COLOR_GOOD, true);
}

void KeyBindingsScreen::onUseDefault() {
	m_selectedKeys = DEFAULT_KEYMAP;
	reload();
}

void KeyBindingsScreen::onReset() {
	m_selectedKeys = g_resourceManager->getConfiguration().mainKeyMap;
	reload();
}