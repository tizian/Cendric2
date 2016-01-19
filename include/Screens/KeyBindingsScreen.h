#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "FileIO/ConfigurationWriter.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "Enums/EnumNames.h"

class KeyBindingsScreen : public Screen {
public:
	KeyBindingsScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	Button* m_backButton = nullptr;
	Button* m_applyButton = nullptr;
	Button* m_useDefaultButton = nullptr;
	Button* m_resetButton = nullptr;
	BitmapText* m_title = nullptr;

	Key m_selectedKey = Key::VOID;

	std::map<Key, std::pair<Button*, sf::Keyboard::Key>> m_keyButtons;
	std::vector<BitmapText*> m_keyTexts;

	std::map < Key, sf::Keyboard::Key > m_selectedKeys;

	void reload();
	// returns true if succeeded, false if not allowed
	// it also reloads the key map if it was successful
	bool trySetKeyBinding(Key key, sf::Keyboard::Key keyboardKey);

	static const std::set<sf::Keyboard::Key> RESERVED_KEYS;
	static const std::set<Key> UNMODIFIABLE_KEYS;
};