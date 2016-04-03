#pragma once

#include "ScreenOverlays/ScreenOverlay.h"
#include "GUI/Window.h"

/* A screen overlay that supports a window with title and subtitle, rendered on top of the screen (in y direction, not z) */
class WindowScreenOverlay : public virtual ScreenOverlay {
public:
	WindowScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	~WindowScreenOverlay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

private:
	Window* m_window;

	void repositionText() override;
};
