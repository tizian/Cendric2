#include "GUI/Cursor.h"
#include "GlobalResource.h"
#include "ResourceManager.h"
#include "InputController.h"

Cursor::Cursor() {
	m_cursorSprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR));
	setCursorSkin(CursorSkin::Pointer);
}

void Cursor::update(const sf::Time& frameTime) {
	m_cursorSprite.setPosition(g_inputController->getDefaultViewMousePosition() - sf::Vector2f(12.f, 12.f));

	if (m_timeUntilNextSkin == sf::Time::Zero) return;
	updateTime(m_timeUntilNextSkin, frameTime);
	if (m_timeUntilNextSkin == sf::Time::Zero) {
		setCursorSkin(m_nextSkin);
	}
}

void Cursor::render(sf::RenderTarget& target) {
	target.draw(m_cursorSprite);
}

void Cursor::setCursorSkin(CursorSkin skin) {
	if (m_timeUntilNextSkin > sf::Time::Zero) return;
	m_cursorSprite.setTextureRect(sf::IntRect(static_cast<int>(skin) * 25, 0, 25, 40));
}

void Cursor::setCursorSkin(CursorSkin skin, sf::Time duration, CursorSkin nextSkin) {
	setCursorSkin(skin);
	m_timeUntilNextSkin = duration;
	m_nextSkin = nextSkin;
}
