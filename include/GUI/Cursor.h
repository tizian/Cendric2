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
	void setCursorSkin(CursorSkin skin);
	void setCursorSkin(CursorSkin skin, const sf::Time& duration, CursorSkin nextSkin);
	void setVisible(bool visible);

	bool isVisible() const;

private:
	sf::Sprite m_cursorSprite;
	sf::Time m_timeUntilNextSkin;
	CursorSkin m_nextSkin;
	sf::Vector2f m_scale;
	bool m_isVisible;
};
