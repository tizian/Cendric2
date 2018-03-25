#pragma once

#include "global.h"
#include "Controller/BaseController.h"

class KeyboardController : public virtual BaseController {
public:
	KeyboardController();

	void update(const sf::Time& frameTime) override;

	// text input
	void startReadingText();
	void stopReadingText();
	const std::string& getReadText() const;
	void cropReadText(int maxLength);
	void readUnicode(sf::Uint32 character);
	void setLastPressedKey(sf::Keyboard::Key key);

	// returns the sf::Keyboard::Key that was pressed in the last frame. If none, returns sf::Keyboard::Unknown
	sf::Keyboard::Key getLastPressedKey() const;

protected:
	bool isKeyboardKeyPressed(Key key) const;

private:
	const std::map<Key, sf::Keyboard::Key>* m_mainKeyMap;
	const std::map<Key, sf::Keyboard::Key>* m_alternativeKeyMap;

	bool isKeyboardKeyPressed(sf::Keyboard::Key key) const;

	bool m_isReadText = false;

	// the text read by the input controller while isReadText is true
	std::string m_readText;

	// keyboard
	sf::Keyboard::Key m_lastPressedKey = sf::Keyboard::Unknown;
};