#include "GUI/Cursor.h"
#include "GlobalResource.h"
#include "ResourceManager.h"
#include "Controller/InputController.h"

Cursor::Cursor() {
	m_cursorSprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR));
	setCursorSkin(Pointer);
	m_scale.x = 1.f;
	m_scale.y = 1.f;
}

void Cursor::update(const sf::Time& frameTime) {
	auto pos = g_inputController->getDefaultViewMousePosition() - sf::Vector2f(12.f, 12.f);
	m_cursorSprite.setPosition(sf::Vector2f(m_scale.x * pos.x, m_scale.y * pos.y));

	if (m_timeUntilNextSkin == sf::Time::Zero) return;
	updateTime(m_timeUntilNextSkin, frameTime);
	if (m_timeUntilNextSkin == sf::Time::Zero) {
		setCursorSkin(m_nextSkin);
	}
}

void Cursor::render(sf::RenderTarget& target) const {
	target.draw(m_cursorSprite);
}

void Cursor::setScale(const sf::Vector2f& scale) {
	m_scale = scale;
	m_cursorSprite.setScale(scale);
}

void Cursor::setCursorSkin(CursorSkin skin) {
	if (m_timeUntilNextSkin > sf::Time::Zero) return;
	m_cursorSprite.setTextureRect(sf::IntRect(static_cast<int>(skin) * 25, 0, 25, 40));
}

void Cursor::setCursorSkin(CursorSkin skin, const sf::Time& duration, CursorSkin nextSkin) {
	setCursorSkin(skin);
	m_timeUntilNextSkin = duration;
	m_nextSkin = nextSkin;
}
