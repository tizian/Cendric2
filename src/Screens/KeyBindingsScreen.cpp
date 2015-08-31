#include "Screens/KeyBindingsScreen.h"
#include "Screens/OptionsScreen.h"

using namespace std;

const std::set<Key> KeyBindingsScreen::UNMODIFIABLE_KEYS = {
	Key::Escape,
	Key::Confirm,
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

KeyBindingsScreen::KeyBindingsScreen(CharacterCore* core) : Screen(core)
{
}

Screen* KeyBindingsScreen::update(const sf::Time& frameTime)
{
	if ((m_selectedKey == Key::VOID && g_inputController->isKeyJustPressed(Key::Escape)) || m_backButton->isClicked())
	{
		return new OptionsScreen(m_characterCore);
	}
	else if (m_selectedKey == Key::VOID && g_inputController->isKeyJustPressed(Key::Escape))
	{
		reload();
	}
	else if (m_selectedKey != Key::VOID && g_inputController->getLastPressedKey() != sf::Keyboard::Unknown)
	{
		if (!trySetKeyBinding(m_selectedKey, g_inputController->getLastPressedKey()))
		{
			setTooltipText(g_textProvider->getText("KeyReserved"), sf::Color::Red, true);
		}
	}
	else if (m_resetButton->isClicked())
	{
		m_selectedKeys = g_resourceManager->getConfiguration().mainKeyMap;
		reload();
	}
	else if (m_useDefaultButton->isClicked())
	{
		m_selectedKeys = DEFAULT_KEYMAP;
		reload();
	}
	else if (m_applyButton->isClicked())
	{
		g_resourceManager->getConfiguration().mainKeyMap = m_selectedKeys;
		ConfigurationWriter writer;
		writer.saveToFile(g_resourceManager->getConfiguration());
		setTooltipText(g_textProvider->getText("ConfigurationSaved"), sf::Color::White, true);
	}
	
	for (auto& it : m_keyButtons)
	{
		if (it.second.first->isClicked())
		{
			it.second.first->setText("PressAnyKey");
			m_selectedKey = it.first;
		}
	}
	
	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
	deleteDisposedObjects();
	return this;
}

void KeyBindingsScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	for (auto it : m_keyTexts)
	{
		renderTarget.draw(*it);
	}
	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);
}

void KeyBindingsScreen::execOnEnter(const Screen *previousScreen)
{
	// title
	m_title = new BitmapText(g_textProvider->getText("KeyBindings"));
	m_title->setCharacterSize(50);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));

	m_selectedKeys = g_resourceManager->getConfiguration().mainKeyMap;

	reload();

	const float buttonWidth = 200.f;
	const float buttonHeight = 50.f;
	const float marginX = 60.f;
	const float marginY = WINDOW_HEIGHT - 100.f;
	const float buttonSpaceWidth = WINDOW_WIDTH - 2 * marginX;
	const float buttonSpacing = (buttonSpaceWidth - 4 * buttonWidth) / 3.f;

	// back
	m_backButton = new Button(sf::FloatRect(marginX, marginY, buttonWidth, buttonHeight));
	m_backButton->setText("Back");
	addObject(m_backButton);
	// reset
	m_resetButton = new Button(sf::FloatRect(marginX + buttonWidth + buttonSpacing, marginY, buttonWidth, buttonHeight));
	m_resetButton->setText("Reset");
	addObject(m_resetButton);
	// default values
	m_useDefaultButton = new Button(sf::FloatRect(marginX + 2 * buttonWidth + 2 * buttonSpacing, marginY, buttonWidth, buttonHeight));
	m_useDefaultButton->setText("Default");
	addObject(m_useDefaultButton);
	// apply
	m_applyButton = new Button(sf::FloatRect(marginX + 3 * buttonWidth + 3 * buttonSpacing, marginY, buttonWidth, buttonHeight));
	m_applyButton->setText("Apply");
	addObject(m_applyButton);
}

bool KeyBindingsScreen::trySetKeyBinding(Key key, sf::Keyboard::Key keyboardKey)
{
	if (RESERVED_KEYS.find(keyboardKey) != RESERVED_KEYS.end())
	{
		reload();
		return false;
	}

	for (auto& it : m_selectedKeys)
	{
		if (it.second == keyboardKey) it.second = sf::Keyboard::KeyCount;
	}
	m_selectedKeys[key] = keyboardKey;
	reload();
	return true;
}

void KeyBindingsScreen::reload()
{
	for (auto& it : m_keyButtons)
	{
		it.second.first->setDisposed();
	}
	m_keyButtons.clear();

	float distFromTop = 150.f;
	float distFromLeft = 150.f;
	const float maxDistFromTop = WINDOW_HEIGHT - 150.f;

	// keyboard mappings
	for (auto& it : m_selectedKeys)
	{
		BitmapText* keyText = new BitmapText(g_textProvider->getText(EnumNames::getKeyName(it.first)));
		keyText->setCharacterSize(16);
		keyText->setPosition(sf::Vector2f(distFromLeft, distFromTop));
		m_keyTexts.push_back(keyText);
		Button* keyButton = new Button(sf::FloatRect(0.f, 0.f, 150.f, 30.f));
		keyButton->setTextRaw(EnumNames::getKeyboardKeyName(it.second), 12);
		if (it.second == sf::Keyboard::KeyCount)
		{
			keyButton->setTextColor(sf::Color::Red);
		}
		keyButton->setPosition(sf::Vector2f(distFromLeft + 300.f, distFromTop));
		if (UNMODIFIABLE_KEYS.find(it.first) != UNMODIFIABLE_KEYS.end())
		{
			keyButton->setEnabled(false);
		}
		m_keyButtons[it.first] = std::pair<Button*, sf::Keyboard::Key>(keyButton, it.second);
		addObject(keyButton);
		distFromTop = distFromTop + 40;
		if (distFromTop > maxDistFromTop)
		{
			distFromTop = 150.f;
			distFromLeft = WINDOW_WIDTH / 2.f + 75.f;
		}
	}

	m_selectedKey = Key::VOID;
}


void KeyBindingsScreen::execOnExit(const Screen *nextScreen)
{
	// delete texts (buttons are deleted automatically by the screen)
	for (auto& it : m_keyTexts)
	{
		delete it;
	}
	m_keyTexts.clear();
	m_keyButtons.clear();
	m_selectedKeys.clear();
}