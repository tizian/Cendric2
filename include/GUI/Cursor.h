#pragma once

#include "global.h"

enum CursorSkin {
	TargetInactive,
	TargetActive,
	TargetHighlight,
	Pointer
};

class Cursor final {
public:
	Cursor();

	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& target) const;

	void setScale(const sf::Vector2f& scale);
	// cursor skins
	void setCursorSkin(CursorSkin skin);
	// param duration: for how long this cursor skin should be displayed
	// until it changes to nextSkin
	void setCursorSkin(CursorSkin skin, const sf::Time& duration, CursorSkin nextSkin);

private:
	sf::Sprite m_cursorSprite;
	sf::Time m_timeUntilNextSkin;
	CursorSkin m_nextSkin;
	sf::Vector2f m_scale;
};
