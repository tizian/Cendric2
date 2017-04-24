#include "GUI/BitmapTextHolder.h"
#include "ResourceManager.h"
#include "GUI/GUIConstants.h"

BitmapTextHolder::BitmapTextHolder(const std::string& str, const sf::Color& col, const sf::Vector2f& posOffset) {
	m_text.setColor(col);
	m_text.setString(str);

	setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_positionOffset = posOffset;
}

void BitmapTextHolder::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_text);
}

void BitmapTextHolder::setCharacterSize(int characterSize) {
	m_text.setCharacterSize(characterSize);

	m_size.x = m_text.getLocalBounds().width;
	m_size.y = m_text.getLocalBounds().height;
}

void BitmapTextHolder::setPosition(const sf::Vector2f& pos) {
	m_text.setPosition(pos + m_positionOffset);
}

const sf::Vector2f& BitmapTextHolder::getPositionOffset() const {
	return m_positionOffset;
}

const sf::Vector2f& BitmapTextHolder::getSize() const {
	return m_size;
}