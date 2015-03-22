#pragma once

#include "stdafx.h"

class ScreenManager
{
public:
	ScreenManager();
	ScreenManager(Screen *initialScreen);

	void update(float dt);
	void render(sf::RenderTarget &renderTarget) const;

	Screen *getCurrentScreen();

private:
	Screen *m_currentScreen = nullptr;
};