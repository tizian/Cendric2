#include "Controller/KeyboardController.h"
#include "ResourceManager.h"

KeyboardController::KeyboardController() {
	m_mainKeyMap = &(g_resourceManager->getConfiguration().mainKeyMap);
	m_alternativeKeyMap = &(g_resourceManager->getConfiguration().alternativeKeyMap);
}

void KeyboardController::update(const sf::Time& frameTime) {
	m_lastPressedKey = sf::Keyboard::Unknown;
}

sf::Keyboard::Key KeyboardController::getLastPressedKey() const {
	return m_lastPressedKey;
}

void KeyboardController::startReadingText() {
	m_readText.clear();
	m_isReadText = true;
}

void KeyboardController::stopReadingText() {
	m_readText.clear();
	m_isReadText = false;
}

const std::string& KeyboardController::getReadText() const {
	return m_readText;
}

void KeyboardController::cropReadText(int maxLength) {
	m_readText = m_readText.substr(0, maxLength);
}

void KeyboardController::readUnicode(sf::Uint32 character) {
	if (!m_isReadText || !m_isWindowFocused
		|| character == '\t'
		|| character == '\n'
		|| character == '\r') return;
	if (character == '\b') {
		if (!m_readText.empty())
			m_readText.erase(m_readText.size() - 1, 1);
	}
	else if (character < 128)
		m_readText += static_cast<char>(character);
}

void KeyboardController::setLastPressedKey(sf::Keyboard::Key key) {
	m_lastPressedKey = key;
}

bool KeyboardController::isKeyboardKeyPressed(Key key) const {
	auto const it = m_mainKeyMap->find(key);
	if (it == m_mainKeyMap->end()) {
		return false;
	}

	return isKeyboardKeyPressed((*it).second) || isKeyboardKeyPressed(m_alternativeKeyMap->at(key));
}

bool KeyboardController::isKeyboardKeyPressed(sf::Keyboard::Key key) const {
	if (key >= sf::Keyboard::KeyCount || key <= sf::Keyboard::Unknown) {
		return false;
	}

	return sf::Keyboard::isKeyPressed(key);
}

