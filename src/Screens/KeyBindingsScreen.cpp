#include "Screens/KeyBindingsScreen.h"
#include "Screens/OptionsScreen.h"

using namespace std;

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
		if (it.second.first->isClicked()) {
			it.second.first->setText("PressAnyKey");
			m_selectedKey = it.first;
		}
	}

	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
}

void KeyBindingsScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	for (auto it : m_keyTexts) {
		renderTarget.draw(*it);
	}
	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);
}

void KeyBindingsScreen::execOnEnter(const Screen *previousScreen) {
	// title
	m_title = new BitmapText(g_textProvider->getText("KeyBindings"));
	m_title->setCharacterSize(GUIConstants::CHARACTER_SIZE_XXL);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 30.f));

	m_selectedKeys = g_resourceManager->getConfiguration().mainKeyMap;

	reload();

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
	for (auto& it : m_keyButtons) {
		it.second.first->setDisposed();
	}
	m_keyButtons.clear();

	float distFromTop = 120.f;
	float distFromLeft = 150.f;
	const float maxDistFromTop = WINDOW_HEIGHT - 130.f;

	// keyboard mappings
	for (auto& it : m_selectedKeys) {
		if (INVISIBLE_KEYS.find(it.first) != INVISIBLE_KEYS.end()) continue;
		BitmapText* keyText = new BitmapText(g_textProvider->getText(EnumNames::getKeyName(it.first)));
		keyText->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
		keyText->setPosition(sf::Vector2f(distFromLeft, distFromTop + 10.f));
		m_keyTexts.push_back(keyText);
		Button* keyButton = new Button(sf::FloatRect(0.f, 0.f, 150.f, 30.f));
		keyButton->setTextRaw(EnumNames::getKeyboardKeyName(it.second), 12);
		if (it.second == sf::Keyboard::KeyCount) {
			keyButton->setTextColor(COLOR_BAD);
		}
		keyButton->setPosition(sf::Vector2f(distFromLeft + 300.f, distFromTop));
		if (UNMODIFIABLE_KEYS.find(it.first) != UNMODIFIABLE_KEYS.end()) {
			keyButton->setEnabled(false);
		}
		m_keyButtons[it.first] = std::pair<Button*, sf::Keyboard::Key>(keyButton, it.second);
		addObject(keyButton);
		distFromTop = distFromTop + 40;
		if (distFromTop > maxDistFromTop) {
			distFromTop = 120.f;
			distFromLeft = WINDOW_WIDTH / 2.f + 75.f;
		}
	}

	m_selectedKey = Key::VOID;
}


void KeyBindingsScreen::execOnExit(const Screen *nextScreen) {
	// delete texts (buttons are deleted automatically by the screen)
	for (auto& it : m_keyTexts) {
		delete it;
	}
	m_keyTexts.clear();
	m_keyButtons.clear();
	m_selectedKeys.clear();
	delete m_title;
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